// Copyright Epic Games, Inc. All Rights Reserved.

#include "FallenCorsairCharacter.h"

#include "Components/Barrel.h"
#include "Components/Gun.h"
#include "Components/Melee.h"
#include "Components/MeleeTargeting.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Materials/MaterialParameterCollection.h"
#include "Kismet/KismetStringLibrary.h"
#include "Player/BrutosMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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
	bUseControllerRotationYaw =  false;
	bUseControllerRotationRoll = false;


	// Configure character movement

	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...

	// GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

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

	GetFollowCamera()->SetRelativeLocation(FVector(0,m_CameraOffset_S.X,m_CameraOffset_S.Y));

	// Create Melee Component
	MeleeComponent = CreateDefaultSubobject<UMelee>(TEXT("MeleeComponnent"));
	MeleeTargetingComponent = CreateDefaultSubobject<UMeleeTargeting>(TEXT("MeleeTargetingComponnent"));

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
	
	OnPlayerSpawn.Broadcast();
}

void AFallenCorsairCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (GetCharacterMovement()->IsMovingOnGround())
	{
		IsStunned = false;
	}



#pragma region Health Recovery
	if(m_currentHealth < m_maxHealth)
	{
		m_currentHealth = m_currentHealth + ((m_recovery / 100)  * m_maxHealth * DeltaTime);
	}
	if(m_currentHealth >= m_maxHealth)
	{
	m_currentHealth = m_maxHealth;
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

#pragma region Melee

	// Chrono for melee input
	if (Melee_IsTrigerred)
	{
		Melee_TriggeredSeconds += DeltaTime;
	}

	if (MeleeComponent->MeleeEnded() && MeleeComponent->MeleeIsHeavy())
	{
		Melee_IsTrigerred = false;
		Melee_TriggeredSeconds = 0;
		MeleeComponent->SetOwnerModeAttack(false);
	}

	if (MeleeTargetingComponent->TargetReached && !MeleeComponent->MeleeIsHeavy())
	{
		MeleeTargetingComponent->TargetReached = false;
		// Perform the first attack combo
		MeleeComponent->ResumeAnimation();
	}

#pragma endregion

	//GetCameraBoom()->TargetArmLength = FMath::Clamp( GetCameraBoom()->TargetArmLength, m_distanceFromPlayer_S / 4, m_distanceFromPlayer_S);
}

void AFallenCorsairCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	// BrutosMovementComponent->AirControl = 0.35f;
	// BrutosMovementComponent->DashPressed();
	
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
		m_direction = -1.f;
	}
}

void AFallenCorsairCharacter::Charge(const FInputActionValue& value)
{
	// if(GetVelocity().Z == 0)
	// {
	// 	BrutosMovementComponent->StopMovementImmediately();
	// 	Jump();
	// 	BrutosMovementComponent->AirControl = 0.f;
	// }

	
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
		EnhancedInputComponent->BindAction(MeleeSoftAction, ETriggerEvent::Started, this, &AFallenCorsairCharacter::MeleeSoftStarted);
		EnhancedInputComponent->BindAction(MeleeHeavyAction, ETriggerEvent::Started, this, &AFallenCorsairCharacter::MeleeHeavyStarted);
		EnhancedInputComponent->BindAction(MeleeHeavyAction, ETriggerEvent::Triggered, this, &AFallenCorsairCharacter::MeleeHeavyTriggered);
		EnhancedInputComponent->BindAction(MeleeHeavyAction, ETriggerEvent::Completed, this, &AFallenCorsairCharacter::MeleeHeavyCompleted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFallenCorsairCharacter::MeleeSetRotation);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AFallenCorsairCharacter::MeleeResetRotation);

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

			// AddControllerYawInput(m_cameraCurve->GetFloatValue(LookAxisVector.X) * yawSensibility);
			// AddControllerPitchInput(m_cameraCurve->GetFloatValue(LookAxisVector.Y) * pitchSensibility);

			AddControllerYawInput(LookAxisVector.X * yawSensibility);
			AddControllerPitchInput(LookAxisVector.Y * pitchSensibility);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Melee

void AFallenCorsairCharacter::MeleeSoftStarted(const FInputActionValue& Value)
{
	if (IsStunned)
		return;

	if(m_bIsFocus)
	{
		gunComp->Shoot();
		
		if (OnShoot.IsBound())
			OnShoot.Broadcast();

		return;
	}

	if (MeleeTargetingComponent->IsMeleeTargeting())
		return;

	if (!MeleeComponent->MeleeIsValid())
		return;


	Melee_IsTrigerred = false;
	Melee_TriggeredSeconds = 0;

	MeleeComponent->SetTypeAttack(EAttackType::Soft);

	if (MeleeComponent->IsFirstCombo())
	{
		// Start the melee targeting if GetTarget() got a valid ennemie to go
		if (MeleeTargetingComponent->GetTarget())
		{
			MeleeComponent->SetOwnerModeAttack(true);
			MeleeComponent->StopAnimationChargingMeleeHeavy();
			MeleeComponent->StartAttack(true, true);
		}
		else 
		{
			MeleeComponent->StartAttack(true);
		}
	}
	else {
		MeleeComponent->PerformAttack();
	}
}

void AFallenCorsairCharacter::MeleeHeavyStarted(const FInputActionValue& Value)
{
	if (IsStunned)
		return;

	if (m_bIsFocus)
		return;

	if (MeleeTargetingComponent->IsMeleeTargeting())
		return;

	if (!MeleeComponent->MeleeIsValid())
		return;

	Melee_IsTrigerred = true;
	MeleeComponent->SetTypeAttack(EAttackType::Heavy);
	MeleeComponent->ResetCombo();
	MeleeComponent->PlayAnimationChargingMeleeHeavy();
	MeleeComponent->SetOwnerModeAttack(true);
	MeleeComponent->UpdateTypeAttack(Melee_TriggeredSeconds);
}

void AFallenCorsairCharacter::MeleeHeavyTriggered(const FInputActionValue& Value)
{
	if (IsStunned)
		return;

	if (m_bIsFocus)
		return;

	if (MeleeTargetingComponent->IsMeleeTargeting())
		return;

	if (!MeleeComponent->MeleeIsValid())
		return;
		
	MeleeComponent->UpdateTypeAttack(Melee_TriggeredSeconds);
}

void AFallenCorsairCharacter::MeleeHeavyCompleted(const FInputActionValue& Value)
{
	if (MeleeComponent->AttackIsStarted())
		return;

	bool bPerformed = MeleeComponent->PerformHeavyAttack(Melee_TriggeredSeconds);
	
	Melee_IsTrigerred = false;
	Melee_TriggeredSeconds = 0;
	MeleeComponent->StopAnimationChargingMeleeHeavy();
	
	if (!bPerformed)
		MeleeComponent->SetOwnerModeAttack(false);
}

void AFallenCorsairCharacter::MeleeSetRotation(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	FVector MovementVector3D = FVector(MovementVector.Y, MovementVector.X, 0);
	MeleeComponent->CalculRotation(MovementVector3D);
	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, UKismetStringLibrary::Conv_Vector2dToString(MovementVector));
}

void AFallenCorsairCharacter::MeleeResetRotation(const FInputActionValue& Value)
{
	MeleeComponent->ResetRotation();
}

//
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////