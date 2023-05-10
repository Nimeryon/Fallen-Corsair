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
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "FallenCorsair/Enemies/AlienBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values for this component's properties
UMelee::UMelee()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	OwnerCharacter = CreateDefaultSubobject<ACharacter>(TEXT("OwnerCharacter"));

	// DeleguateMeleeSoft.AddDynamic(this, &UMelee::OnMyDelegateTriggered);
}

void UMelee::OnMyDelegateTriggered()
{
	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("Deleguate Melee triggered"));
}

// Called when the game starts
void UMelee::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());

	if (character)
	{
		OwnerCharacter = character;
		MaxWalkSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
		MinWalkSpeed = OwnerCharacter->GetCharacterMovement()->MinAnalogWalkSpeed;

		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		
		if (AnimInstance)
		{
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UMelee::OnNotifyBeginReceived);
			AnimInstance->OnMontageEnded.AddDynamic(this, &UMelee::OnMontageEnded);
			AnimInstance->OnMontageBlendingOut.AddDynamic(this, &UMelee::OnMontageBlendOut);
		}

	}
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

bool UMelee::PerformHeavyAttack(float& eslapsedSeconds)
{
	if (!MeleeIsValid())
		return false;

	if (!AttackIsStarted())
	{
		if (eslapsedSeconds >= delayInputDepthMeleeHeavy)
		{
			SetTypeAttack(EAttackType::Heavy);
			StartAttack(true);
			return true;
		}
	}

	return false;
}

void UMelee::PlayAnimationChargingMeleeHeavy()
{
	if (!AnimWhileChargingMeleeHeavy || indexCurrentAttack > 0)
		return;

	// Play Animation
	if (OwnerCharacter->GetMesh())
	{

		if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(AnimWhileChargingMeleeHeavy);
			MontageToPlay = AnimWhileChargingMeleeHeavy;
		}
	}
}

void UMelee::StopAnimationChargingMeleeHeavy()
{
	if (!AnimWhileChargingMeleeHeavy || !AnimWhileChargingMeleeHeavyLoop)
		return;

	// Stop Animation
	if (OwnerCharacter->GetMesh())
	{
		if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
		{
			if (AnimInstance->Montage_IsPlaying(AnimWhileChargingMeleeHeavy))
			{
				AnimInstance->StopAllMontages(0.0f);
			}
			else if (AnimInstance->Montage_IsPlaying(AnimWhileChargingMeleeHeavyLoop))
			{
				AnimInstance->StopAllMontages(0.0f);
			}
		}
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
	if (!MeleeIsValid())
		return;
		
	if (ModeAttack)
	{
		FreezeRotation(true);
		EnableWalk(false);
	}
	else {
		FreezeRotation(false);
		EnableWalk(true);
		ResetCombo();
		StartAttack(false);
	}

	bMeleeEnded = false;

}

void UMelee::StartAttack(bool start, bool _bFreezeAnimation)
{
	bAttackStarted = start;
	bFreezeAnimation = _bFreezeAnimation;

	if (start && indexCurrentAttack == 0)
	{
		PerformAttack();
	}
	
	if (!start){
		attackType = EAttackType::Soft;
	}
}

void UMelee::UpdateTypeAttack(float& eslapsedSeconds)
{
	if (!AttackIsStarted())
	{
		if (eslapsedSeconds <= delayInputDepthMeleeHeavy)
		{
			// SetTypeAttack(EAttackType::Soft);
			bMeleeHeavyChargeCompleted = false;
		}
		else {
			SetTypeAttack(EAttackType::Heavy);

			if (!bMeleeHeavyChargeCompleted)
				SpawnFX(PS_MeleeHeavyOnChargeCompleted);

			bMeleeHeavyChargeCompleted = true;

			if (!MeleeIsValid())
			{
				SetTypeAttack(EAttackType::Soft);
			}
		}
	}
}

void UMelee::ResetRotation()
{
	FRotator Dir;
	Dir = UKismetMathLibrary::Conv_VectorToRotator(OwnerCharacter->GetActorForwardVector());
	RotatorWhileAttackStarted = Dir;
}

void UMelee::ResumeAnimation()
{
	if (OwnerCharacter->GetMesh())
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			if (MontageToPlay)
			{
				AnimInstance->Montage_Resume(MontageToPlay);
			}
		}
	}
}

void UMelee::SetReleased(bool released)
{
	bInputReleased = released;
}

void UMelee::CancelAttack()
{
	ResetCombo();
	SetOwnerModeAttack(false);
	StartAttack(false);
	bMeleeEnded = false;

	if (OwnerCharacter->GetMesh())
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			if (MontageToPlay)
			{
				AnimInstance->Montage_Stop(0, MontageToPlay);
			}
		}
	}
}

void UMelee::CalculRotation(FVector _rot)
{
	AFallenCorsairCharacter* c = Cast<AFallenCorsairCharacter>(OwnerCharacter);

	if (!c)
		return;

	FVector rot = c->GetCameraBoom()->GetTargetRotation().RotateVector(_rot);
	rot.Normalize();
	FRotator rotation = UKismetMathLibrary::MakeRotFromX(rot);
	RotatorWhileAttackStarted = FRotator(0, rotation.Yaw, 0);
}

void UMelee::PlaySoundCharge()
{
	if (SoundCharge)
		UGameplayStatics::SpawnSound2D(GetWorld(), SoundCharge);
}

bool UMelee::AttackIsStarted() const
{
	return bAttackStarted;
}

bool UMelee::MeleeIsHeavy() const
{
	return attackType == EAttackType::Heavy;
}

bool UMelee::MeleeEnded() const
{
	return bMeleeEnded;
}

bool UMelee::MeleeIsValid() const
{
	if (!OwnerCharacter->GetCharacterMovement()->IsMovingOnGround() && !CanAirAttack)
	{
		return false;
	}
		
	switch (attackType)
	{
	case EAttackType::Soft:
		if (Melees.Soft.Num() > 0 && indexCurrentAttack < Melees.Soft.Num())
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
		if (Melees.Heavy.Num() > 0 && indexCurrentAttack < Melees.Heavy.Num())
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
		if (Melees.Soft.Num() > 0 && indexCurrentAttack < Melees.Soft.Num())
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

bool UMelee::IsFirstCombo() const
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

bool UMelee::IsLastCombo() const
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Protected

void UMelee::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, UKismetStringLibrary::Conv_NameToString(NotifyName));

	if (NotifyName == "PoseStarting")
	{
		// if (!bFreezeAnimation)
		// 	return;
			// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("lol"));

		if (OwnerCharacter->GetMesh())
		{
			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				if (MontageToPlay)
				{
					AnimInstance->Montage_Pause(MontageToPlay);
				}
			}
		}
	}	
	if (NotifyName == "Propulsion")
	{
		PropulseOwner();
	}	
	else if (NotifyName == "StopPropulsion")
	{
		AFallenCorsairCharacter *FallenCorsairCharacter = Cast<AFallenCorsairCharacter>(GetOwner());
	
		if (FallenCorsairCharacter)
		{
			if (!FallenCorsairCharacter->IsStunned)
				ResetVelocity();
		}
		else 
			ResetVelocity();
	}
	else if (NotifyName == "Hit")
	{
		if (!GetCurrentMelee().CollisionWithSockets)
			TriggerHitWithCollisionShape();

		if (attackType == EAttackType::Heavy)
			SpawnFX(PS_MeleeHeavyOnHit);
	}	 
	else if (NotifyName == "CanCombo")
	{
		if (IsLastCombo())
		{
			StartAttack(false);
		}
		else
		{
			IncrementCurrentAttack();
			if (bExecuteNextAttack)
			{
				bExecuteNextAttack = false;
				AttackSequence(); // Execute next attack
			}
			else
			{
				bCanAttack = true;
			}
		}
	}
	else if (NotifyName == "CanMove")
	{
		FreezeRotation(false);
		EnableWalk(true);

		if (IsLastCombo())
		{
			ResetCombo();
			StartAttack(false);
			SetReleased(true);
		}

		bMeleeEnded = true;
	}	
	else if (NotifyName == "Recovery")
	{
		bMeleeEnded = true;
		SetOwnerModeAttack(false);
	}
	else if (NotifyName == "HitSound")
	{
		if (GetCurrentMelee().AttackSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), GetCurrentMelee().AttackSound, 1, 1, 0);
		}
	}
	else if (NotifyName == "VoiceSound")
	{
		if (GetCurrentMelee().PlayerVoiceSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), GetCurrentMelee().PlayerVoiceSound, 1, 1, 0);
		}

		if (attackType == EAttackType::Heavy)
			PlaySoundCharge();
	}
}

void UMelee::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}

void UMelee::OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if ((MontageToPlay == AnimWhileChargingMeleeHeavy && !bInterrupted))
	{
		// Play Animation
		if (OwnerCharacter->GetMesh())
		{
			if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
			{
				// Activer la lecture en boucle
				AnimInstance->Montage_Play(AnimWhileChargingMeleeHeavyLoop);
				MontageToPlay = AnimWhileChargingMeleeHeavyLoop;
			}
		}
	}
}

void UMelee::DammageOnHits(TArray<FHitResult> OutHits)
{
	for (auto It = OutHits.CreateIterator(); It; It++)
	{
		ACharacter* CharacterHited = Cast<ACharacter>((*It).GetActor());

		if (CharacterHited)
		{
			if (CharactersHited.Contains(CharacterHited))
			{
				continue;
			}

			// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, CharacterHited->GetName());

			
			CharactersHited.Add(CharacterHited);

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

			float DammageValue = 0;
			float Distance;
			
			switch (attackType)
			{
				case EAttackType::Soft:
					DammageValue = GetCurrentMelee().Dammage;
					break;
				case EAttackType::Heavy:

					Distance = FVector::Distance(CharacterHited->GetActorLocation(), GetOwner()->GetActorLocation());
				
					if (Distance >=  GetCurrentMelee().MaxDistance)
					{
						DammageValue = GetCurrentMelee().MinDammage;
						break;
					}
				
					if (Distance <= GetCurrentMelee().MinDistance)
					{
						DammageValue = GetCurrentMelee().Dammage;
						break;
					}
			
					DammageValue = GetCurrentMelee().MinDammage + (GetCurrentMelee().Dammage - GetCurrentMelee().MinDammage) * (1 - UKismetMathLibrary::NormalizeToRange(Distance, GetCurrentMelee().MinDistance, GetCurrentMelee().MaxDistance));
					break;
				default:
					DammageValue = GetCurrentMelee().Dammage;
					break;
			}

			DammageValue = UKismetMathLibrary::FCeil(DammageValue);
			
			// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, CharacterHited->GetName());
			FDamageTypeEvent DamageEvent;

			switch (attackType)
			{
			case EAttackType::Soft:
					DamageEvent.DamageType = EDamageType::MeleeSoft;
					break;
				case EAttackType::Heavy:
					DamageEvent.DamageType = EDamageType::MeleeHeavy;
					break;
				default:
					DamageEvent.DamageType = EDamageType::MeleeSoft;
			}
 
			// FDamageEvent EventDamage;
			// CharacterHited->TakeDamage(DammageValue, EventDamage, nullptr, GetOwner());
			CharacterHited->TakeDamage(DammageValue, DamageEvent, nullptr, GetOwner());
		}
	}
}

//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private

void UMelee::TriggerHitWithCollisionShape()
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

	switch (GetCurrentMelee().CollisionShape)
	{
		case EMeleeCollisionShape::Box:
			GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, Orientation.Quaternion(), UEngineTypes::ConvertToTraceType(ECC_Visibility), BoxShape, QueryParams);
			break;
		case EMeleeCollisionShape::Sphere:
			GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, Orientation.Quaternion(), UEngineTypes::ConvertToTraceType(ECC_Visibility), SphereShape, QueryParams);
			break;
		case EMeleeCollisionShape::Capsule:
			GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, Orientation.Quaternion(), UEngineTypes::ConvertToTraceType(ECC_Visibility), CapsuleShape, QueryParams);
			break;
		default:
			GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, Orientation.Quaternion(), UEngineTypes::ConvertToTraceType(ECC_Visibility), BoxShape, QueryParams);
	}

	if (Debug)
	{
		FColor Color = FColor::Red;
		if (OutHits.Num())
			Color = FColor::Green;
		switch (GetCurrentMelee().CollisionShape)
		{
			case EMeleeCollisionShape::Box:
				DrawDebugBox(GetWorld(), Start, GetCurrentMelee().BoxSize, Orientation.Quaternion(), Color, false, 1, 0, 1);
				break;
			case EMeleeCollisionShape::Sphere:
				DrawDebugSphere(GetWorld(), Start, GetCurrentMelee().SphereRadius, 10, Color, false, 1, 0, 1);
				break;
			case EMeleeCollisionShape::Capsule:
				DrawDebugCapsule(GetWorld(), Start, GetCurrentMelee().CapsuleHalfHeight, GetCurrentMelee().CapsuleRadius, Orientation.Quaternion(), Color, false, 1, 0, 1);
				break;
			default:
				DrawDebugBox(GetWorld(), Start, GetCurrentMelee().BoxSize, Orientation.Quaternion(), Color, false, 1, 0, 1);
		}
	}

	DammageOnHits(OutHits);
}

void UMelee::TriggerHitWithSockets()
{
	if (!GetCurrentMelee().CollisionWithSockets)
	{
		return;
	}

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FVector StartLocation = OwnerCharacter->GetMesh()->GetSocketLocation(GetCurrentMelee().SocketStart);;
	FVector EndLocation = OwnerCharacter->GetMesh()->GetSocketLocation(GetCurrentMelee().SocketEnd);
	float SphereRadius = GetCurrentMelee().SocketSphereRadius; // The radius of the sphere you want to trace
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereRadius);

	// Perform the multi-sphere trace operation
	TArray<FHitResult> OutHits;
	GetWorld()->SweepMultiByObjectType(OutHits, StartLocation, EndLocation, FQuat::Identity, UEngineTypes::ConvertToTraceType(ECC_Visibility), SphereShape, QueryParams);

	if (Debug)
	{
		FColor Color = FColor::Red;

		if (OutHits.Num())
			Color = FColor::Green;

		DrawDebugSphere(GetWorld(), StartLocation, SphereRadius, 10, Color, false, 1.f, 0, 1.f);
		DrawDebugSphere(GetWorld(), EndLocation, SphereRadius, 10, Color, false, 1.f, 0, 1.f );
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, 1, 0, 1);
	}


	DammageOnHits(OutHits);
}

// Character

void UMelee::SetRotation()
{
	OwnerCharacter->SetActorRotation(RotatorWhileAttackStarted);
	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, UKismetStringLibrary::Conv_RotatorToString(RotatorWhileAttackStarted));

}

void UMelee::FreezeRotation(bool freeze)
{
	OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = !freeze;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = !freeze;
}

void UMelee::EnableWalk(bool enable)
{
	if (enable)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		OwnerCharacter->GetCharacterMovement()->MinAnalogWalkSpeed = MinWalkSpeed;
	}
	else {
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 0;
		OwnerCharacter->GetCharacterMovement()->MinAnalogWalkSpeed = 0;

	}
}

void UMelee::ResetVelocity()
{
	OwnerCharacter->GetCharacterMovement()->Velocity = FVector(0, 0, 0);
}

void UMelee::PropulseOwner()
{
	GetCurrentMelee().PropulsionDirectionOwner.Normalize();

	FVector Start = GetOwner()->GetActorLocation();
	FVector End = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * GetCurrentMelee().PropulsionDirectionOwner.X + GetOwner()->GetActorRightVector() * GetCurrentMelee().PropulsionDirectionOwner.Y + GetOwner()->GetActorUpVector() * GetCurrentMelee().PropulsionDirectionOwner.Z;
	FVector Dir = End - Start;

	Dir.Normalize();

	FVector Force = Dir * GetCurrentMelee().PropulsionForceOwner;

	OwnerCharacter->GetCharacterMovement()->AddImpulse(Force, true);
}

// Attack
void UMelee::AttackSequence()
{
	CharactersHited.Reset();

	if (attackType == EAttackType::Soft)
		SetRotation();
		
	EnableWalk(false);
	bMeleeEnded = false;

	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, UKismetStringLibrary::Conv_IntToString(indexCurrentAttack));

	// Play Animation
	if (OwnerCharacter->GetMesh())
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		
		if (AnimInstance)
		{
			bool bPlayedSuccessfully = false;
			MontageToPlay = GetCurrentMelee().Anim;
			const float MontageLength = AnimInstance->Montage_Play(MontageToPlay);
			
			bPlayedSuccessfully = (MontageLength > 0.f);

			if (bPlayedSuccessfully)
			{
				switch (attackType)
				{
				case EAttackType::Soft:
					if (DeleguateMeleeSoft.IsBound())
						DeleguateMeleeSoft.Broadcast();
					break;
				case EAttackType::Heavy:
					if (DeleguateMeleeHeavy.IsBound())
						DeleguateMeleeHeavy.Broadcast();
					break;
				default:
					if (DeleguateMeleeSoft.IsBound())
						DeleguateMeleeSoft.Broadcast();
					break;
				}
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

void UMelee::SpawnFX(UParticleSystem* FX)
{
	FVector SpawnLocation = GetOwner()->GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FTransform SpawnTransform(SpawnRotation, SpawnLocation);
	FVector Scale = FVector(1, 1, 1);

	// Niagara FX
	// if (FX)
	// 	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX, SpawnLocation, SpawnRotation,  Scale, true);

	// Cascade particule system
	if (FX)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FX, SpawnTransform, true);
}



//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////