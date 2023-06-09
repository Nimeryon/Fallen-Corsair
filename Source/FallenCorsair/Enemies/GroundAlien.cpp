// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundAlien.h"

#include "AIController.h"
#include "DropSoul.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "FallenCorsair/FallenCorsairCharacter.h"
#include "FallenCorsair/Components/DashComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void AGroundAlien::BeginPlay()
{
	Super::BeginPlay();

	m_material = GetMesh()->CreateDynamicMaterialInstance(0, nullptr, "Stitch Material");
	m_material->SetScalarParameterValue("AngryLevel", 1);
	m_material->SetScalarParameterValue("Shift", m_hueColor);

	AAIController* AIController = Cast<AAIController>(GetController());
	UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
	BlackBoard->SetValueAsFloat("MinAttackJumpDistance", m_attackMinJumpDistance);
	BlackBoard->SetValueAsFloat("MaxAttackJumpDistance", m_attackMaxJumpDistance);

	m_scaleVariation = UKismetMathLibrary::RandomFloatInRange(m_minScaleVariation, m_maxScaleVariation);
	SetActorRelativeScale3D(GetActorScale() + FVector(m_scaleVariation));
	
	GetCharacterMovement()->JumpZVelocity = UKismetMathLibrary::RandomFloatInRange(m_attackMinJumpForce, m_attackMaxJumpForce);
	GetCharacterMovement()->RotationRate.Yaw = m_rotationSpeed;
	m_attackTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void AGroundAlien::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!m_bIsInCooldown) return;

	m_currentCooldownTime += DeltaSeconds;
	if (m_currentCooldownTime >= m_cooldownTime)
	{
		m_bIsInCooldown = false;
		m_currentCooldownTime = 0;
	}

	if (m_bmulticolor)
	{
		m_hueColor += m_hueShiftSpeed * DeltaSeconds;
		if (m_hueColor >= 100)
			m_hueColor = 0;

		m_material->SetScalarParameterValue("Shift", m_hueColor);
	}
}

void AGroundAlien::PrepareAttack()
{
	if (!m_attackTarget) return;
	
	m_attackTargetPosition = m_attackTarget->GetActorLocation();
}

bool AGroundAlien::Attack()
{
	if (!m_attackTarget) return false;

	const FVector Position = GetActorLocation() + GetActorForwardVector() * m_attackBoxOffset;
	FHitResult Hit;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAlienBase::StaticClass(), ActorsToIgnore);

	UKismetSystemLibrary::BoxTraceSingleByProfile(
		GetWorld(),
		Position,
		Position,
		m_attackBoxSize / 2.f,
		GetActorRotation(),
		"AlienAttack",
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true
	);

	// if (Hit.GetActor())
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());

	if (Hit.GetActor() == m_attackTarget)
	{
		SetCooldownTime(m_hitCooldownTime);

		FDamageEvent DamageEvent;
		Hit.GetActor()->TakeDamage(m_attackDamage, DamageEvent, nullptr, this);
	}
	else SetCooldownTime(m_missCooldownTime);
	
	return true;
}

bool AGroundAlien::CreateAvoidBox()
{
	if (!m_attackTarget) return false;
	
	const FVector Position = GetActorLocation() + GetActorForwardVector() * m_avoidBoxOffset;
	FHitResult Hit;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAlienBase::StaticClass(), ActorsToIgnore);

	UKismetSystemLibrary::BoxTraceSingleByProfile(
		GetWorld(),
		Position,
		Position,
		m_avoidBoxSize / 2.f,
		GetActorRotation(),
		"AlienAttack",
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		Hit,
		true,
		FColor::Blue,
		FColor::Orange
	);

	// if (Hit.GetActor())
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());

	if (Hit.GetActor() == m_attackTarget)
	{
		AFallenCorsairCharacter* Character = Cast<AFallenCorsairCharacter>(Hit.GetActor());
		if (Character)
			Character->dashComp->SetCanPerfectDodge();
	}
	
	return true;
}

void AGroundAlien::JumpTowardsTarget()
{
	const FVector Direction = (m_attackTargetPosition - GetActorLocation()).GetSafeNormal();

	GetCapsuleComponent()->SetCollisionProfileName("AlienAttack");
	
	if (UKismetMathLibrary::RandomFloat() >= m_attackAngleVariationChance)
	{
		const FVector RandomDirection = Direction.RotateAngleAxis(UKismetMathLibrary::RandomFloatInRange(-m_attackRandomAngleVariation, m_attackRandomAngleVariation), FVector::UpVector);
		GetCharacterMovement()->AddImpulse(RandomDirection * UKismetMathLibrary::RandomFloatInRange(m_attackMinJumpSpeed, m_attackMaxJumpSpeed), true);
	}
	else GetCharacterMovement()->AddImpulse(Direction * UKismetMathLibrary::RandomFloatInRange(m_attackMinJumpSpeed, m_attackMaxJumpSpeed), true);
	Jump();
}

bool AGroundAlien::IsInCooldown() const { return m_bIsInCooldown; }

void AGroundAlien::SetCooldownTime(float Cooldown) { m_cooldownTime = Cooldown; }

void AGroundAlien::SetCooldownActive() { m_bIsInCooldown = true; }

AlienState AGroundAlien::GetState() const { return m_state; }

void AGroundAlien::ForceDeath()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("InstantAbort"), true);
}

void AGroundAlien::SetState(AlienState State) { m_state = State; }

void AGroundAlien::Death(EDamageType DamageType)
{
	m_killDamageType = DamageType;

	m_material->SetScalarParameterValue("AngryLevel", 0);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("InstantAbort"), true);
	
	Super::Death(DamageType);
}

bool AGroundAlien::Stun(float Time)
{
	const bool IsStunned = Super::Stun(Time);
	if (IsStunned) m_material->SetScalarParameterValue("AngryLevel", 0);

	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->RotationRate.Yaw = 0;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetCharacterMovement()->StopMovementImmediately();
	
	GetCapsuleComponent()->SetCollisionProfileName("AlienGround");

	PlayStunMontage();

	return IsStunned;
}

void AGroundAlien::Unstunned()
{
	m_material->SetScalarParameterValue("AngryLevel", 1);

	GetCharacterMovement()->MaxWalkSpeed = m_movementSpeed;
	GetCharacterMovement()->RotationRate.Yaw = m_rotationSpeed;
	
	GetMesh()->GetAnimInstance()->Montage_Stop(m_blendTime);
}

void AGroundAlien::PlayStunMontage()
{
	if (m_stunMontage)
	{
		FAlphaBlendArgs BlendArgs;
		BlendArgs.BlendTime = m_blendTime;
		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(m_stunMontage))
			GetMesh()->GetAnimInstance()->Montage_PlayWithBlendIn(m_stunMontage, BlendArgs);
	}
}

void AGroundAlien::AttachAttackFX()
{
	if (!m_attackParticle) return;
	
	for (const FName& SocketName : m_attackSocketsName)
	{
		if (GetMesh()->DoesSocketExist(SocketName))
		{
			const FVector SpawnLocation = GetMesh()->GetSocketLocation(SocketName);
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				m_attackParticle,
				GetRootComponent(),
				SocketName,
				SpawnLocation,
				FRotator::ZeroRotator,
				EAttachLocation::KeepWorldPosition,
				true,
				true
			);
		}
	}
}

void AGroundAlien::ResetCollisionPresset()
{
	GetCapsuleComponent()->SetCollisionProfileName("AlienGround");
}

void AGroundAlien::PlayDeathFX()
{
	GetMesh()->DestroyComponent();
	Super::PlayDeathFX();
	
	if (m_killDamageType == EDamageType::MeleeHeavy || m_killDamageType == EDamageType::MeleeSoft)
		GetWorld()->SpawnActor<ADropSoul>(m_soul, GetActorLocation(), FRotator::ZeroRotator);
}
