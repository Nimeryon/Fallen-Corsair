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
#include "Kismet/GameplayStatics.h"

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
	{
		ownerCharacter = character;
		MaxWalkSpeed = ownerCharacter->GetCharacterMovement()->MaxWalkSpeed;
	}
}

void UMelee::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
		SetOwnerModeAttack(true);
		AttackSequence();
	}
	else if (!IsLastCombo())
	{
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

void UMelee::SetOwnerModeAttack(bool ModeAttack)
{
	if (ModeAttack)
	{
		FreezeRotation(true);
		EnableWalk(false);
	}
	else {
		FreezeRotation(false);
		EnableWalk(false);
		ResetCombo();
		StartAttack(false);
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

void UMelee::UpdateTypeAttack(float& eslapsedSeconds)
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
			if (!MeleeIsValid())
			{
				SetTypeAttack(EAttackType::Soft);
			}

			SetReleased(true);
			StartAttack(true);
		}
	}
}

void UMelee::ResetRotation()
{
	FRotator Dir;
	Dir = UKismetMathLibrary::Conv_VectorToRotator(ownerCharacter->GetActorForwardVector());
	RotatorWhileAttackStarted = Dir;
}

bool UMelee::MeleeIsValid()
{
	switch (attackType)
	{
	case EAttackType::Soft:
		if (Melees.Soft.Num() > 0)
		{
			if (Melees.Soft[0].Anim)
			{
				return true;
			}
			return false;
		}
		return false;
		break;
	case EAttackType::Heavy:
		if (Melees.Heavy.Num() > 0)
		{
			if (Melees.Heavy[0].Anim)
			{
				return true;
			}
			return false;
		}
		return false;
		break;
	default:
		if (Melees.Soft.Num() > 0)
		{
			if (Melees.Soft[0].Anim)
			{
				return true;
			}
			return false;
		}
		return false;
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
	ResetCombo();
}

bool UMelee::AttackIsStarted()
{
	return bAttackStarted;
}

void UMelee::CalculRotation(FVector _rot)
{
	AFallenCorsairCharacter* c = Cast<AFallenCorsairCharacter>(ownerCharacter);
	FVector rot = c->GetCameraBoom()->GetTargetRotation().RotateVector(_rot);
	rot.Normalize();
	FRotator rotation = UKismetMathLibrary::MakeRotFromX(rot);
	RotatorWhileAttackStarted = FRotator(0, rotation.Yaw, 0);
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
	if (NotifyName == "StopPropulsion")
	{
		ResetVelocity();
	}
	else if (NotifyName == "Hit")
	{
		TriggerHit();
	}	
	// else if (NotifyName == "CanCombo")
	// {
	// 	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1); // Reset to normal time dilation

	// 	if (IsLastCombo())
	// 	{
	// 		StartAttack(false);
	// 	}
	// 	else
	// 	{
	// 		IncrementCurrentAttack();
	// 		if (bExecuteNextAttack)
	// 		{
	// 			bExecuteNextAttack = false;
	// 			// Execute next attack
	// 			AttackSequence();
	// 		}
	// 		else
	// 		{
	// 			bCanAttack = true;
	// 		}
	// 	}
	// }
	// else if (NotifyName == "CanMove")
	// {
	// 	FreezeRotation(false);
	// 	EnableWalk(true);
	// }	
	// else if (NotifyName == "Recovery")
	// {
	// 	ResetCombo();
	// 	StartAttack(false);
	// }
	// else if (NotifyName == "HitSound")
	// {
	// 	if (GetCurrentMelee().AttackSound)
	// 	{
	// 		UGameplayStatics::PlaySound2D(GetWorld(), GetCurrentMelee().AttackSound, 1, 1, 0);
	// 	}
	// }
	// else if (NotifyName == "VoiceSound")
	// {
	// 	if (GetCurrentMelee().PlayerVoiceSound)
	// 	{
	// 		UGameplayStatics::PlaySound2D(GetWorld(), GetCurrentMelee().PlayerVoiceSound, 1, 1, 0);
	// 	}
	// }
}

void UMelee::TriggerHit()
{
	FVector OffsetPos = GetOwner()->GetActorForwardVector() * GetCurrentMelee().CollisionShapeOffset.X + GetOwner()->GetActorRightVector() * GetCurrentMelee().CollisionShapeOffset.Y + GetOwner()->GetActorUpVector() * GetCurrentMelee().CollisionShapeOffset.Z;
	FVector Start = GetOwner()->GetActorLocation() + OffsetPos;
	FRotator Orientation = UKismetMathLibrary::MakeRotFromX(GetOwner()->GetActorForwardVector());
	Orientation += GetCurrentMelee().CollisionShapeRotation;

	FCollisionShape BoxShape = FCollisionShape::MakeBox(GetCurrentMelee().BoxSize);
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(GetCurrentMelee().SphereRadius);
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(GetCurrentMelee().CapsuleRadius, GetCurrentMelee().CapsuleHalfHeight);

	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	bool bHitSomething = false;
	
	switch (GetCurrentMelee().CollisionShape)
	{
		case EMeleeCollisionShape::Box:
			bHitSomething = GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, Orientation.Quaternion(), UEngineTypes::ConvertToTraceType(ECC_Visibility), BoxShape, QueryParams);
			if (Debug)
				DrawDebugBox(GetWorld(), Start, GetCurrentMelee().BoxSize, Orientation.Quaternion(), FColor::Red, false, 1, 0, 1);
			break;
		case EMeleeCollisionShape::Sphere:
			bHitSomething = GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, Orientation.Quaternion(), UEngineTypes::ConvertToTraceType(ECC_Visibility), SphereShape, QueryParams);
			if (Debug)
				DrawDebugSphere(GetWorld(), Start, GetCurrentMelee().SphereRadius, 10, FColor::Red, false, 1, 0, 1);
			break;
		case EMeleeCollisionShape::Capsule:
			bHitSomething = GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, Orientation.Quaternion(), UEngineTypes::ConvertToTraceType(ECC_Visibility), CapsuleShape, QueryParams);
			if (Debug)
				DrawDebugCapsule(GetWorld(), Start, GetCurrentMelee().CapsuleHalfHeight, GetCurrentMelee().CapsuleRadius, Orientation.Quaternion(), FColor::Red, false, 1, 0, 1);
			break;
		default:
			bHitSomething = GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, Orientation.Quaternion(), UEngineTypes::ConvertToTraceType(ECC_Visibility), BoxShape, QueryParams);
			if (Debug)
				DrawDebugBox(GetWorld(), Start, GetCurrentMelee().BoxSize, Orientation.Quaternion(), FColor::Red, false, 1, 0, 1);
	}
	
	if (bHitSomething)
	{
		for (auto It = OutHits.CreateIterator(); It; It++)
		{
			ACharacter* CharacterHited = Cast<ACharacter>((*It).GetActor());

			if (CharacterHited)
			{
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), GetCurrentMelee().TimeDilationOnHit);

				if (Debug)
				{
					switch (GetCurrentMelee().CollisionShape)
					{
						case EMeleeCollisionShape::Box:
							DrawDebugBox(GetWorld(), Start, GetCurrentMelee().BoxSize, Orientation.Quaternion(), FColor::Green, false, 1, 0, 1);
							break;
						case EMeleeCollisionShape::Sphere:
							DrawDebugSphere(GetWorld(), Start, GetCurrentMelee().SphereRadius, 10, FColor::Green, false, 1, 0, 1);
							break;
						case EMeleeCollisionShape::Capsule:
							DrawDebugCapsule(GetWorld(), Start, GetCurrentMelee().CapsuleHalfHeight, GetCurrentMelee().CapsuleRadius, Orientation.Quaternion(), FColor::Green, false, 1, 0, 1);
							break;
						default:
							DrawDebugBox(GetWorld(), Start, GetCurrentMelee().BoxSize, Orientation.Quaternion(), FColor::Green, false, 1, 0, 1);
					}
				}

				// Propulse ennemie
				GetCurrentMelee().PropulsionDirectionEnnemie.Normalize();

				FVector End, Dir;

				if (GetCurrentMelee().PropulsionEnnemieDirectionFromOwner)
				{
					Dir = CharacterHited->GetActorLocation() -  GetOwner()->GetActorLocation();
					if (GetCurrentMelee().PropulsionEnnemieDirectionFromOwnerNormalize2D)
					{
						Dir = FVector(Dir.X, Dir.Y, 0);
					}
				}
				else {
					End = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * GetCurrentMelee().PropulsionDirectionEnnemie.X + GetOwner()->GetActorRightVector() * GetCurrentMelee().PropulsionDirectionEnnemie.Y + GetOwner()->GetActorUpVector() * GetCurrentMelee().PropulsionDirectionEnnemie.Z;
					Dir = End - GetOwner()->GetActorLocation();
				}

				Dir.Normalize();
				FVector Force = Dir * GetCurrentMelee().PropulsionForceEnnemie;
				CharacterHited->GetCharacterMovement()->AddImpulse(Force, true);

				// Damage Target
				FDamageEvent eventDamage;
				CharacterHited->TakeDamage(GetCurrentMelee().Dammage, eventDamage, nullptr, GetOwner());
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
	ownerCharacter->SetActorRotation(RotatorWhileAttackStarted);
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
		ownerCharacter->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
	else {
		ownerCharacter->GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void UMelee::ResetVelocity()
{
	ownerCharacter->GetCharacterMovement()->Velocity = FVector(0, 0, 0);
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

// Attack

void UMelee::AttackSequence()
{
	SetRotation();

	bool bPlayedSuccessfully = false;
	UAnimMontage* MontageToPlay = GetCurrentMelee().Anim;

	// Play Animation
	if (ownerCharacter->GetMesh())
	{
		if (UAnimInstance* AnimInstance = ownerCharacter->GetMesh()->GetAnimInstance())
		{
			const float MontageLength = AnimInstance->Montage_Play(MontageToPlay, 2);
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
					AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMelee::OnNotifyBeginReceived);
				}

				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UMelee::OnNotifyBeginReceived);
				bIsDeleguate = true;

			}
		}
	}

	bCanExecuteNextAttack = true;
}

FAttackData& UMelee::GetCurrentMelee()
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

bool UMelee::IsFirstCombo()
{
	switch (attackType)
	{
	case EAttackType::Soft:
		return indexCurrentAttack == 0;
		break;
	case EAttackType::Heavy:
		return indexCurrentAttack == 0;
		break;
	default:
		return indexCurrentAttack == 0;
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
