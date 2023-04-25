// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundAlien.h"

#include "FallenCorsair/FallenCorsairCharacter.h"
#include "FallenCorsair/Components/Melee.h"
#include "FallenCorsair/Waves/WaveZone.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AGroundAlien::AGroundAlien(const FObjectInitializer& ObjectInitializer)
{
	m_meleeComponent = CreateDefaultSubobject<UMelee>(TEXT("MeleeComponnent"));
}

void AGroundAlien::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->JumpZVelocity = m_attackJumpForce;
	m_attackTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
}

bool AGroundAlien::Attack()
{
	const FVector Forward = GetActorForwardVector();
	//UKismetSystemLibrary::DrawDebugBox(GetWorld(), GetActorLocation() + Forward * m_attackBoxOffset, m_attackBoxSize, FLinearColor::Yellow, FRotator::ZeroRotator, 1, 1);
	
	if (!m_attackTarget) return false;

	const FVector Position = GetActorLocation() + GetActorForwardVector() * m_attackBoxOffset;
	FHitResult Hit;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveZone::StaticClass(), ActorsToIgnore);

	UKismetSystemLibrary::BoxTraceSingleByProfile(
		GetWorld(),
		Position,
		Position,
		m_attackBoxSize / 2.f,
		GetActorRotation(),
		TEXT("ECC_EngineTraceChannel1"),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		Hit,
		true
	);

	if (Hit.GetActor() == m_attackTarget)
	{
		
	}
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
	const FVector Direction = (m_attackTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	
	GetCharacterMovement()->AddImpulse(Direction * m_attackJumpSpeed, true);
	Jump();
}

AlienState AGroundAlien::GetState() const { return m_state; }

void AGroundAlien::SetState(AlienState State) { m_state = State; }

UAnimInstance* AGroundAlien::GetAnimInstance() const
{
	return GetMesh()->GetAnimInstance();
}
