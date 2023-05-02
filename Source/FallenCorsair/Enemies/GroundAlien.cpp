// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundAlien.h"

#include "DropSoul.h"
#include "FallenCorsair/FallenCorsairCharacter.h"
#include "FallenCorsair/Components/Melee.h"
#include "FallenCorsair/Waves/WaveZone.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AGroundAlien::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->JumpZVelocity = m_attackJumpForce;
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
		"Player",
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		Hit,
		true
	);

	if (Hit.GetActor())
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());

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
	const FVector Forward = GetActorForwardVector();
	//UKismetSystemLibrary::DrawDebugBox(GetWorld(), GetActorLocation() + Forward * m_avoidBoxOffset, m_avoidBoxSize, FLinearColor::Blue, FRotator::ZeroRotator, 1.5, 1);

	return true;
}

void AGroundAlien::JumpTowardsTarget()
{
	const FVector Direction = (m_attackTargetPosition - GetActorLocation()).GetSafeNormal();
	
	GetCharacterMovement()->AddImpulse(Direction * m_attackJumpSpeed, true);
	Jump();
}

bool AGroundAlien::IsInCooldown() const { return m_bIsInCooldown; }

void AGroundAlien::SetCooldownTime(float Cooldown) { m_cooldownTime = Cooldown; }

void AGroundAlien::SetCooldownActive() { m_bIsInCooldown = true; }

AlienState AGroundAlien::GetState() const { return m_state; }

void AGroundAlien::SetState(AlienState State) { m_state = State; }

void AGroundAlien::Death(EDamageType DamageType)
{
	if (DamageType == EDamageType::MeleeHeavy || DamageType == EDamageType::MeleeSoft)
		GetWorld()->SpawnActor<ADropSoul>(m_soul, GetActorLocation(), FRotator::ZeroRotator);
	
	Super::Death(DamageType);
}
