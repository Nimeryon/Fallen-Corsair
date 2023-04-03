// Copyright Epic Games, Inc. All Rights Reserved.

#include "FallenCorsairCharacter.h"

#include "Barrel.h"
#include "Gun.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FrameTypes.h"
#include "Kismet/KismetMathLibrary.h"



//////////////////////////////////////////////////////////////////////////
// AFallenCorsairCharacter

AFallenCorsairCharacter::AFallenCorsairCharacter()
{
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
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	barrelComp = CreateDefaultSubobject<UBarrel>(TEXT("BarrelComponnent"));
	gunComp = CreateDefaultSubobject<UGun>(TEXT("GunComponnent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AFallenCorsairCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
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

	if(m_bIsFocus)
	{
		FRotator newRot;
		newRot.Roll = GetActorRotation().Roll;
		newRot.Pitch = GetActorRotation().Pitch;
		newRot.Yaw = GetCameraBoom()->GetTargetRotation().Yaw;
		SetActorRotation(newRot);
	}

	
	m_alpha = FMath::Clamp( m_alpha + (1 / m_transitionSpeed * m_direction) * DeltaTime, 0, 1);

	if((m_alpha != 0) || (m_alpha != 1))
	{
		GetCameraBoom()->TargetArmLength = FMath::InterpEaseIn(400, 0, m_alpha, 2);

		FVector A = FVector(0.f,0.f,0.f);
		FVector B = FVector(-40.f, 30.f, 70.f);
		FVector newLoc;
		newLoc.X = FMath::InterpEaseIn(A.X, B.X, m_alpha, 2);
		newLoc.Y = FMath::InterpEaseIn(A.Y, B.Y, m_alpha, 2);
		newLoc.Z = FMath::InterpEaseIn(A.Z, B.Z, m_alpha, 2);
		
		GetCameraBoom()->SetRelativeLocation(newLoc);
	}
	
}

void AFallenCorsairCharacter::Shoot()
{
	if(m_bIsFocus)
		gunComp->Shoot();
}

void AFallenCorsairCharacter::Aim(const FInputActionValue& bIsZoom)
{
	m_bIsFocus = bIsZoom.Get<bool>();
	GetCameraBoom()->bEnableCameraLag = !m_bIsFocus;
	
	if(m_bIsFocus)
		m_direction = 1.f;
	else
		m_direction = -1.f;
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

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AFallenCorsairCharacter::Shoot);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AFallenCorsairCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AFallenCorsairCharacter::Aim);
		
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
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




