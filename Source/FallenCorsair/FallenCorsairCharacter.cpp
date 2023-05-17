// Copyright Epic Games, Inc. All Rights Reserved.

#include "FallenCorsairCharacter.h"

#include "Components/Barrel.h"
#include "Components/Gun.h"
#include "Components/DashComponent.h"
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
#include "VectorUtil.h"
#include "Blueprint/UserWidget.h"

#include "Camera/CameraShakeBase.h"
#include "Materials/MaterialParameterCollection.h"
#include "Kismet/KismetStringLibrary.h"
#include "Player/BrutosMovementComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AFallenCorsairCharacter

/// you can focus while you are attacking you have to fix


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
	// Create Dash Component
	dashComp = CreateDefaultSubobject<UDashComponent>(TEXT("DashComponnent"));

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
	GetCharacterMovement()->MaxWalkSpeed = m_walkSpeed;
	
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
	
	float BrutosOpacity = UKismetMathLibrary::MapRangeClamped(FVector::Distance(GetFollowCamera()->GetComponentLocation(), GetActorLocation()), m_opacityDistanceMin, m_opacityDistanceMax, 0, 1);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_collection, "Opacity", BrutosOpacity);

#pragma region Health Gestion
	if(m_currentHealth < m_maxHealth)
	{
		m_currentHealth = m_currentHealth + ((m_recovery / 100)  * m_maxHealth * DeltaTime);
	}
	if(m_currentHealth >= m_maxHealth)
	{
		m_currentHealth = m_maxHealth;
	}

	if(m_currentHealth < m_lowHP)
	{
		m_bIsHealing = true;
		m_bIsLowHP = true;
		m_alphaDerecover = FMath::Clamp(m_alphaDerecover + (1 / m_changeSpeed) * DeltaTime, 0.f, 1.f);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_collection, "bIsLowHP", m_alphaDerecover);
		m_alphaRecover = 1.f;
	}
	else if(m_bIsLowHP)
	{
		m_alphaDerecover = 0;
		m_alphaRecover = FMath::Clamp(m_alphaRecover - (1 / m_changeSpeed) * DeltaTime, 0.f, 1.f);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_collection, "bIsLowHP", m_alphaRecover);
		if(m_alphaRecover == 0.f)
			m_bIsHealing = false;
	}
	else
		m_bIsLowHP = false;

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
		if(m_bIsCharge && m_bIsFocus && m_alpha == 1 && m_alphaCharge < 1 && barrelComp->GetSlot() > 0)
		{
			m_alphaCharge = FMath::Clamp( m_alphaCharge + (1 / 1) * DeltaTime, 0, 1);
			GetCameraBoom()->TargetArmLength = FMath::InterpEaseIn(m_distanceFromPlayer_A, m_distanceFromPlayer_C, m_alphaCharge, 2);
			GetFollowCamera()->SetFieldOfView(FMath::InterpEaseIn(m_fieldOfView_A, m_fieldOfView_C, m_alphaCharge, 2));
			if(m_cameraShake)
				m_cameraManager->StartCameraShake(m_cameraShake, m_alphaCharge, ECameraShakePlaySpace::CameraLocal);
		}
		else if(m_alphaCharge >= 1 && m_bIsFocus && m_bIsCharge)
		{
			if(m_cameraShake)
				m_cameraManager->StopCameraShake(m_currentCameraShake, true);
		}
		else if(m_alphaCharge > 0)
		{
			if(m_cameraShake)
				m_cameraManager->StopCameraShake(m_currentCameraShake, true);

			m_bIsCharge = false;
			m_alphaCharge = FMath::Clamp( m_alphaCharge + (1 / -0.1) * DeltaTime, 0, 1);
			GetCameraBoom()->TargetArmLength = FMath::InterpEaseIn(m_distanceFromPlayer_A, m_distanceFromPlayer_C, m_alphaCharge, 2);
			GetFollowCamera()->SetFieldOfView(FMath::InterpEaseIn(m_fieldOfView_A, m_fieldOfView_C, m_alphaCharge, 2));
		}
		else
		{
			m_alphaCharge = 0;
			FVector2D newLoc = FMath::InterpEaseIn(m_CameraOffset_S, m_CameraOffset_A, m_alpha, 2);
			
		
			GetCameraBoom()->TargetArmLength = FMath::InterpEaseIn(m_distanceFromPlayer_S, m_distanceFromPlayer_A, m_alpha, 2);
			GetCameraBoom()->SetRelativeLocation(FVector(0,newLoc.X,newLoc.Y));
			GetFollowCamera()->SetFieldOfView(FMath::InterpEaseIn(m_fieldOfView_S, m_fieldOfView_A, m_alpha, 2));
			m_cameraManager->ViewPitchMin = FMath::InterpEaseIn(m_pitchMin_S, m_pitchMin_A, m_alpha, 2);
			m_cameraManager->ViewPitchMax = FMath::InterpEaseIn(m_pitchMax_S, m_pitchMax_A, m_alpha, 2);
		}
		if (!MeleeComponent->MeleeIsHeavy() && !MeleeComponent->AttackIsStarted())
		{
			if(m_bIsFocus && m_bIsCharge)
			{
				GetCharacterMovement()->MaxWalkSpeed = m_chargeWalkSpeed;
			}
			else if(m_bIsFocus)
			{
				GetCharacterMovement()->MaxWalkSpeed = m_aimWalkSpeed;
			}
			else
			{
				GetCharacterMovement()->MaxWalkSpeed = m_walkSpeed;
			}
			
			if(m_bIsFocus)
			{
				FRotator newRot;
				newRot.Roll = GetActorRotation().Roll;
				newRot.Pitch = GetActorRotation().Pitch;
				newRot.Yaw = GetCameraBoom()->GetTargetRotation().Yaw - 45;
				SetActorRotation(newRot);
			}
		}
	}
#pragma endregion

#pragma region Melee
	// Chrono for melee input
	if (Melee_IsTrigerred)
	{
		Melee_TriggeredSeconds += DeltaTime;
	}

	if (MeleeComponent && MeleeComponent->MeleeEnded() && MeleeComponent->MeleeIsHeavy())
	{
		Melee_IsTrigerred = false;
		Melee_TriggeredSeconds = 0;
		MeleeComponent->SetOwnerModeAttack(EAttackMode::Normal);
	}

	if (MeleeTargetingComponent && MeleeTargetingComponent->TargetReached && !MeleeComponent->MeleeIsHeavy())
	{
		MeleeTargetingComponent->TargetReached = false;
		// Perform the first attack combo
		MeleeComponent->ResumeAnimation();
	}
#pragma endregion
}

float AFallenCorsairCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_bCanBeDammage)
	{
		m_currentHealth = FMath::Clamp(m_currentHealth - DamageAmount, 0, m_maxHealth);
		Hurt = true;
	}

	if (m_currentHealth <= 0)
	{
		PlayerDeath();
		if (SoundDeath)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundDeath, GetActorLocation());
			SoundDeath = nullptr;
		}
	}
	else
	{
		if (SoundGetHurt)
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundGetHurt, GetActorLocation());
	}

	// UE_LOG(LogTemp, Warning, TEXT("Brutos life: %f"), m_currentHealth);
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AFallenCorsairCharacter::Aim(const FInputActionValue& bIsZoom)
{
	if(!bIsAimAvailable)
		return;
	
	OnAim.Broadcast();
	m_bIsFocus = bIsZoom.Get<bool>();
	GetCameraBoom()->bEnableCameraLag = !m_bIsFocus;
	
	if(m_bIsFocus)
	{
		m_direction = 1.f;
	}
	else
	{
		if(gunComp)
		{
			gunComp->StopCharge(true);
			m_direction = -1.f;
		}
	}
}

void AFallenCorsairCharacter::Charge(const FInputActionValue& value)
{
	if(!bIsDashAvailable)
		return;
	
	if(!m_bIsFocus && !MeleeComponent->AttackIsStarted() && !MeleeComponent->MeleeIsHeavy())
	{
		bool bDashSuccessed = dashComp->DashPressed();
		OnDodge.Broadcast();
	}
}

TArray<AActor*> AFallenCorsairCharacter::SetIgnoreCharacterActors()
{
	TArray<AActor*> ActorToIgnore;
	TArray<AActor*> CharacterChildren;
	ActorToIgnore.Add(this);
	GetAllChildActors(CharacterChildren);

	return ActorToIgnore;
}

void AFallenCorsairCharacter::SetCanBeDamage(bool bCanBeDammage)
{
	m_bCanBeDammage = bCanBeDammage;
}

void AFallenCorsairCharacter::PlayerDeath()
{
	if(!SpawnedDeathWidget && m_deathHUD)
	{
		UUserWidget* Widget = Cast<UUserWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(), m_deathHUD));
		Widget->AddToViewport();

		SpawnedDeathWidget = true;
	}
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
		EnhancedInputComponent->BindAction(MeleeHeavyAction, ETriggerEvent::Completed, this, &AFallenCorsairCharacter::ShootCompleted);
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
			AddControllerYawInput(m_cameraCurve->GetFloatValue(LookAxisVector.X) * yawSensibility);
			AddControllerPitchInput(m_cameraCurve->GetFloatValue(LookAxisVector.Y) * pitchSensibility);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Melee

void AFallenCorsairCharacter::MeleeSoftStarted(const FInputActionValue& Value)
{
	if(!bIsSoftMeleeAvailable)
		return;
	
	if (IsStunned)
		return;

	if(m_bIsFocus)
		return;

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
			MeleeComponent->SetOwnerModeAttack(EAttackMode::None);
			MeleeComponent->StopAnimationChargingMeleeHeavy();
			MeleeComponent->StartAttack(true);
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
	if(!bIsHeavyMeleeAvailable)
		return;
	
	if (IsStunned)
		return;

	if(m_bIsFocus)
	{
		if(!bIsShootAvailable)
			return;
		
		m_bIsCharge = Value.Get<bool>();
		gunComp->Shoot();
		
		if (OnShoot.IsBound())
		{
			OnShoot.Broadcast();
			
#if WITH_EDITOR
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow, FString::Printf(TEXT("onshoot broadcast")));
#endif
		}

		return;
	}

	if (MeleeTargetingComponent->IsMeleeTargeting())
		return;

	if (!MeleeComponent->MeleeIsValid())
		return;
	
	if (!MeleeComponent->CanAttack())
		return;

	Melee_IsTrigerred = true;
	MeleeComponent->SetTypeAttack(EAttackType::Heavy);
	MeleeComponent->ResetCombo();
	MeleeComponent->PlayAnimationChargingMeleeHeavy();
	MeleeComponent->SetOwnerModeAttack(EAttackMode::NoMove);
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
	
	if (!MeleeComponent->CanAttack())
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
		MeleeComponent->SetOwnerModeAttack(EAttackMode::Normal);
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

void AFallenCorsairCharacter::ShootCompleted(const FInputActionValue& Value)
{
	if (m_bIsFocus)
	{
		m_bIsCharge = Value.Get<bool>();
		gunComp->StopCharge(false);
	}
}

//
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
