// Copyright Epic Games, Inc. All Rights Reserved.

#include "FallenCorsairCharacter.h"

#include "Components/Barrel.h"
#include "Components/Gun.h"
#include "Components/DashComponent.h"
#include "Components/Melee.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "VectorUtil.h"
#include "Materials/MaterialParameterCollection.h"
#include "Player/BrutosMovementComponent.h"
#include "Kismet/KismetMaterialLibrary.h"


//////////////////////////////////////////////////////////////////////////
// AFallenCorsairCharacter

AFallenCorsairCharacter::AFallenCorsairCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UBrutosMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	BrutosMovementComponent = Cast<UBrutosMovementComponent>(GetCharacterMovement());
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = m_distanceFromPlayer_S; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create Barrel Component
	barrelComp = CreateDefaultSubobject<UBarrel>(TEXT("BarrelComponnent"));
	// Create Gun Component
	gunComp = CreateDefaultSubobject<UGun>(TEXT("GunComponnent"));
	// Create Dash Component
	dashComp = CreateDefaultSubobject<UDashComponent>(TEXT("DashComponnent"));

	GetFollowCamera()->SetRelativeLocation(FVector(0,m_CameraOffset_S.X,m_CameraOffset_S.Y));

	// Create Melee Component
	MeleeComponent = CreateDefaultSubobject<UMelee>(TEXT("MeleeComponnent"));



	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AFallenCorsairCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	/// I place the camera lag here because it doesn't work inthje constructor dunno why **confuse smiley**
	GetCameraBoom()->CameraLagSpeed = m_cameraLag;
	
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	m_cameraManager= PlayerController->PlayerCameraManager;
	m_cameraManager->ViewPitchMin = m_pitchMin_S;
	m_cameraManager->ViewPitchMax = m_pitchMax_S;
	GetFollowCamera()->SetRelativeRotation(FRotator(m_pitchAngle,0,0));

	//Add Input Mapping Context
	if (PlayerController == Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AFallenCorsairCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Chrono for melee input
	if (Melee_IsTrigerred)
		Melee_TriggeredSeconds += DeltaTime;

#pragma region Health Gestion
	if(m_currentHealth < m_maxHealth)
	{
		m_currentHealth = m_currentHealth + ((m_recovery / 100)  * m_maxHealth * DeltaTime);
	}
	if(m_currentHealth >= m_maxHealth)
	{
		m_currentHealth = m_maxHealth;
	}

	if(m_currentHealth < 20)
	{
		m_bIsHealing = true;
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_collection, "bIsLowHP", 1.f);
		m_alphaRecover = 1.f;
	}
	else if(m_bIsHealing)
	{
		m_alphaRecover = FMath::Clamp(m_alphaRecover - (1 / m_changeSpeed) * DeltaTime, 0.f, 1.f);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_collection, "bIsLowHP", m_alphaRecover);
		if(m_alphaRecover == 0.f)
			m_bIsHealing = false;
	}

#pragma endregion 

#pragma region Camera Zoom
	float transition;
	if(m_bIsFocus)
		transition = m_transitionSpeedZoom;
	else
		transition = m_transitionSpeedDezoom;
	
	m_alpha = FMath::Clamp( m_alpha + (1 / transition * m_direction) * DeltaTime, 0, 1);

	if((m_alpha != 0) || (m_alpha != 1))
	{
		FVector2D newLoc = FMath::InterpEaseIn(m_CameraOffset_S, m_CameraOffset_A, m_alpha, 2);
		
		GetCameraBoom()->TargetArmLength = FMath::InterpEaseIn(m_distanceFromPlayer_S, m_distanceFromPlayer_A, m_alpha, 2);
		GetCameraBoom()->SetRelativeLocation(FVector(0,newLoc.X,newLoc.Y));
		GetFollowCamera()->SetFieldOfView(FMath::InterpEaseIn(m_fieldOfView_S, m_fieldOfView_A, m_alpha, 2));
		m_cameraManager->ViewPitchMin = FMath::InterpEaseIn(m_pitchMin_S, m_pitchMin_A, m_alpha, 2);
		m_cameraManager->ViewPitchMax = FMath::InterpEaseIn(m_pitchMax_S, m_pitchMax_A, m_alpha, 2);

		if(m_bIsFocus)
		{
			FRotator newRot;
			newRot.Roll = GetActorRotation().Roll;
			newRot.Pitch = GetActorRotation().Pitch;
			newRot.Yaw = GetCameraBoom()->GetTargetRotation().Yaw;
			SetActorRotation(newRot);
		}
	}
#pragma endregion
}

void AFallenCorsairCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	// BrutosMovementComponent->AirControl = 0.35f;
	// BrutosMovementComponent->DashPressed();
	
}

float AFallenCorsairCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(m_currentHealth > DamageAmount)
	{
		m_currentHealth = m_currentHealth - DamageAmount;
	}
	else
	{
		m_currentHealth = 0;
		/// called death event
	}

	
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AFallenCorsairCharacter::Aim(const FInputActionValue& bIsZoom)
{
	OnAim.Broadcast();
	m_bIsFocus = bIsZoom.Get<bool>();
	GetCameraBoom()->bEnableCameraLag = !m_bIsFocus;
	
	if(m_bIsFocus)
	{
		m_direction = 1.f;
	}
	else
	{
		gunComp->StopCharge(true);
		m_direction = -1.f;
	}
}

void AFallenCorsairCharacter::Charge(const FInputActionValue& value)
{
	//BrutosMovementComponent->DashPressed();
	dashComp->DashPressed();
}

TArray<AActor*> AFallenCorsairCharacter::SetIgnoreCharacterActors()
{
	TArray<AActor*> ActorToIgnore;
	TArray<AActor*> CharacterChildren;
	ActorToIgnore.Add(this);
	GetAllChildActors(CharacterChildren);

	return ActorToIgnore;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFallenCorsairCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFallenCorsairCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFallenCorsairCharacter::Look);

		//Melee
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &AFallenCorsairCharacter::MeleeTriggered);
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &AFallenCorsairCharacter::MeleeStarted);
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Completed, this, &AFallenCorsairCharacter::MeleeCompleted);

		// Shoot
		//EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AFallenCorsairCharacter::MeleeStarted);

		// Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AFallenCorsairCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AFallenCorsairCharacter::Aim);

		EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Started, this, &AFallenCorsairCharacter::Charge);
		//EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Completed, this, &AFallenCorsairCharacter::Charge);
	}

}

void AFallenCorsairCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFallenCorsairCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		
		float yawSensibility;
		float pitchSensibility;
		
		if(m_bIsFocus)
		{
			yawSensibility = m_mouseYawSensitivity_A;
			pitchSensibility = m_mousePitchSensitivity_A;
		}
		else
		{
			yawSensibility = m_mouseYawSensitivity_S;
			pitchSensibility = m_mousePitchSensitivity_S;
		}
		if(m_cameraCurve)
		{
			AddControllerYawInput(m_cameraCurve->GetFloatValue(LookAxisVector.X) * yawSensibility);
			AddControllerPitchInput(m_cameraCurve->GetFloatValue(LookAxisVector.Y) * pitchSensibility);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Melee

void AFallenCorsairCharacter::MeleeTriggered(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, UKismetStringLibrary::Conv_BoolToString(MeleeComponent->MeleeIsValid()));

	if(!m_bIsFocus)
	{
		if (!MeleeComponent->MeleeIsValid())
			return;

		if (!MeleeComponent->IsReleased())
		{
			if (!MeleeComponent->AttackIsStarted()) {
				Melee_IsTrigerred = true;
				MeleeComponent->UpdateTypeAttack(Melee_TriggeredSeconds);
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, UKismetStringLibrary::Conv_FloatToString(Melee_TriggeredSeconds));
	}
	
}

void AFallenCorsairCharacter::MeleeStarted(const FInputActionValue& Value)
{

	if(m_bIsFocus)
	{
		gunComp->Shoot();
		OnShoot.Broadcast();
	}
	else
	{
		if (!MeleeComponent->MeleeIsValid())
			return;

		if (MeleeComponent->AttackIsStarted()) {
			MeleeComponent->PerformAttack();
		}

		MeleeComponent->SetReleased(false);
	}
	
}

void AFallenCorsairCharacter::MeleeCompleted(const FInputActionValue& Value)
{
	if(!m_bIsFocus)
	{
		if (!MeleeComponent->MeleeIsValid())
			return;

		Melee_IsTrigerred = false;
		Melee_TriggeredSeconds = 0;

		if (!MeleeComponent->IsReleased())
		{
			MeleeComponent->StartAttack(true);
		}
	}
	else
	{
		gunComp->StopCharge(false);
	}
}

//
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////