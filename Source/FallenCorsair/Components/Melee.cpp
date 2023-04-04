// Fill out your copyright notice in the Description page of Project Settings.


#include "Melee.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../FallenCorsairCharacter.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/Vector2D.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Math/Quat.h"

// Sets default values for this component's properties
UMelee::UMelee()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ownerCharacter = CreateDefaultSubobject<ACharacter>(TEXT("ownerCharacter"));


	// ...
}


// Called when the game starts
void UMelee::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());

	if (character)
		ownerCharacter = character;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public

void UMelee::PerformAttack()
{
	if (!MeleeIsValid())
		return;

	if (bCanAttack)
	{
		bCanAttack = false;
		FreezeRotation(true);
		EnableWalk(false);
		AttackSequence();
	} else {
		bExecuteNextAttack = bCanExecuteNextAttack;
	}
}

void UMelee::SetTypeAttack(EAttackType at)
{
	if (!AttackIsStarted())
	{
		attackType = at;
	}
}

void UMelee::StartAttack(bool start)
{
	bAttackStarted = start;
	if (start && indexCurrentAttack == 0)
	{
		PerformAttack();
	}
}

void UMelee::UpdateTypeAttack(float &eslapsedSeconds)
{
	if (!AttackIsStarted())
	{
		if (eslapsedSeconds <= delayInputDepthMeleeHeavy)
		{
			SetTypeAttack(EAttackType::Soft);
		}
		else {
			eslapsedSeconds = 0;
			SetTypeAttack(EAttackType::Heavy);
			SetReleased(true);
			StartAttack(true);
		}
	}
}

bool UMelee::IsReleased() const
{
	return bInputReleased;
}

void UMelee::SetReleased(bool released)
{
	bInputReleased = released;
}

void UMelee::CancelAttack()
{
	GetCurrentMelee().Anim;
	ownerCharacter->GetMesh()->GetAnimInstance()->StopAllMontages(0);
	ResetState();
}

bool UMelee::AttackIsStarted()
{
	return bAttackStarted;
}


//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Protected

void UMelee::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "Propulsion")
	{
		PropulseOwner();
	}
	else if (NotifyName == "Hit")
	{
		TriggerHit();
		ResetVelocity();
	}
	else if (NotifyName == "Recovery")
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, UKismetStringLibrary::Conv_BoolToString(IsLastCombo()));
		
		if (IsLastCombo())
		{
			StartAttack(false);
		}
		else {
			if (bExecuteNextAttack)
			{
				bExecuteNextAttack = false;
				IncrementCurrentAttack();
				AttackSequence(); // Execute next attack
			}
		}
	}
	else if (NotifyName == "Completed")
	{
		ResetState();
	}
	// Sound
	else if (NotifyName == "Hit_Soft")
	{

	}
	else if (NotifyName == "Hit_Heavy")
	{
	}
}

void UMelee::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AnimDeleguate->OnMontageEnded.RemoveDynamic(this, &UMelee::OnMontageEnded);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("ended"));
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, UKismetStringLibrary::Conv_IntToString(indexCurrentAttack));
	ResetState();
}

void UMelee::TriggerHit()
{
	FVector OffsetPos = GetOwner()->GetActorForwardVector() * GetCurrentMelee().BoxOffset.X + GetOwner()->GetActorRightVector() * GetCurrentMelee().BoxOffset.Y + GetOwner()->GetActorUpVector() * GetCurrentMelee().BoxOffset.Z;
	FVector Start = GetOwner()->GetActorLocation() + OffsetPos;
	FRotator Orientation = UKismetMathLibrary::MakeRotFromX(GetOwner()->GetActorForwardVector());
	FCollisionShape BoxShape = FCollisionShape::MakeBox(GetCurrentMelee().BoxSize);
	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	bool bHitSomething = GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, Orientation.Quaternion(), UEngineTypes::ConvertToTraceType(ECC_Visibility), BoxShape, QueryParams);
	
	if (Debug)
		DrawDebugBox(GetWorld(), Start, GetCurrentMelee().BoxSize, FColor::Purple, false, 1, 0, 1);
	
	if (bHitSomething)
	{
		for (auto It = OutHits.CreateIterator(); It; It++)
		{
			ACharacter* character = Cast<ACharacter>((*It).GetActor());
			if (character)
			{
				// Propulse ennemie
				GetCurrentMelee().PropulsionDirectionEnnemie.Normalize();
				FVector End = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * GetCurrentMelee().PropulsionDirectionEnnemie.X + GetOwner()->GetActorRightVector() * GetCurrentMelee().PropulsionDirectionEnnemie.Y + GetOwner()->GetActorUpVector() * GetCurrentMelee().PropulsionDirectionEnnemie.Z;
				FVector Dir = End - GetOwner()->GetActorLocation();
				Dir.Normalize();
				FVector Force = Dir * GetCurrentMelee().PropulsionForceEnnemie;
				character->GetCharacterMovement()->AddImpulse(Force, true);

				// Damage Target
				FDamageEvent eventDamage;
				character->TakeDamage(GetCurrentMelee().Dammage, eventDamage, nullptr, GetOwner());
			}
		}
	}
}

//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private

// Character

void UMelee::SetRotation()
{
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(rotation);
	ownerCharacter->SetActorRotation(MovementRotation);
}

void UMelee::CalculRotation(FVector _rot)
{
	AFallenCorsairCharacter* c = Cast<AFallenCorsairCharacter>(ownerCharacter);
	FVector rot = c->GetCameraBoom()->GetTargetRotation().RotateVector(_rot);
	rot.Normalize();
	rotation = rot;
}

void UMelee::FreezeRotation(bool freeze)
{
	ownerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = !freeze;
	ownerCharacter->GetCharacterMovement()->bOrientRotationToMovement = !freeze;
}

void UMelee::EnableWalk(bool enable)
{
	if (enable)
	{
		ownerCharacter->GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed;
	}
	else {
		maxWalkSpeed = ownerCharacter->GetCharacterMovement()->MaxWalkSpeed;
		ownerCharacter->GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

// Attack

void UMelee::AttackSequence()
{
	//SetRotation();
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, UKismetStringLibrary::Conv_IntToString(indexCurrentAttack));

	bool bPlayedSuccessfully = false;
	UAnimMontage* MontageToPlay = GetCurrentMelee().Anim;
	
	if (ownerCharacter->GetMesh())
	{
		if (UAnimInstance* AnimInstance = ownerCharacter->GetMesh()->GetAnimInstance())
		{
			const float MontageLength = AnimInstance->Montage_Play(MontageToPlay);
			bPlayedSuccessfully = (MontageLength > 0.f);
			if (bPlayedSuccessfully)
			{
				UAnimInstance* AnimInstancePtr = AnimInstance;
				if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay))
				{
					int MontageInstanceID = MontageInstance->GetInstanceID();
				}

				if (bIsDeleguate)
				{
					AnimDeleguate->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMelee::OnNotifyBeginReceived);
				}

				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UMelee::OnNotifyBeginReceived);
				bIsDeleguate = true;

				//AnimInstance->OnMontageEnded.AddDynamic(this, &UMelee::OnMontageEnded);
				AnimDeleguate = AnimInstance;
			}
		}
	}

	bCanExecuteNextAttack = true;
}

void UMelee::PropulseOwner()
{
	GetCurrentMelee().PropulsionDirectionOwner.Normalize();

	FVector Start = GetOwner()->GetActorLocation();
	FVector End = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * GetCurrentMelee().PropulsionDirectionOwner.X + GetOwner()->GetActorRightVector() * GetCurrentMelee().PropulsionDirectionOwner.Y + GetOwner()->GetActorUpVector() * GetCurrentMelee().PropulsionDirectionOwner.Z;
	FVector Dir = End - Start;

	Dir.Normalize();

	FVector Force = Dir * GetCurrentMelee().PropulsionForceOwner;

	ownerCharacter->GetCharacterMovement()->AddImpulse(Force, true);
}
	

void UMelee::ResetVelocity()
{
	ownerCharacter->GetCharacterMovement()->Velocity = FVector(0, 0, 0);
}

FAttackData &UMelee::GetCurrentMelee()
{
	switch (attackType)
	{
		case EAttackType::Soft:
			return Melees.Soft[indexCurrentAttack];
			break;
		case EAttackType::Heavy:
			return Melees.Heavy[indexCurrentAttack];
			break;
		default:
			return Melees.Soft[indexCurrentAttack];
	}
}

void UMelee::ResetCombo()
{
	indexCurrentAttack = 0;
	bCanAttack = true;
}

// Reset all component
void UMelee::ResetState()
{
	ResetCombo();
	FreezeRotation(false);
	EnableWalk(true);
	StartAttack(false);
}

void UMelee::IncrementCurrentAttack()
{
	indexCurrentAttack++;
	switch (attackType)
	{
	case EAttackType::Soft:
		indexCurrentAttack %= Melees.Soft.Num();
		break;
	case EAttackType::Heavy:
		indexCurrentAttack %= Melees.Heavy.Num();
		break;
	default:
		indexCurrentAttack %= Melees.Soft.Num();
	}
}

bool UMelee::MeleeIsValid()
{
	switch (attackType)
	{
	case EAttackType::Soft:
		return Melees.Soft.Num() > 0;
		break;
	case EAttackType::Heavy:
		return Melees.Heavy.Num() > 0;
		break;
	default:
		return Melees.Soft.Num() > 0;
	}
}

bool UMelee::IsLastCombo()
{
	switch (attackType)
	{
	case EAttackType::Soft:
		return indexCurrentAttack == Melees.Soft.Num() - 1;
		break;
	case EAttackType::Heavy:
		return indexCurrentAttack == Melees.Heavy.Num() - 1;
		break;
	default:
		return indexCurrentAttack == Melees.Soft.Num() - 1;
	}
}

//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



