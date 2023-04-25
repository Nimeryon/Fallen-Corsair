// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundAlien.h"
#include "FallenCorsair/Components/Melee.h"
#include "Kismet/KismetSystemLibrary.h"

void AGroundAlien::Destroyed()
{
	Super::Destroyed();

	UAnimInstance* AnimInstance = GetAnimInstance();
	if (AnimInstance)
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AGroundAlien::OnNotifyReceived);
}

void AGroundAlien::BeginPlay()
{
	Super::BeginPlay();

	m_attackTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void AGroundAlien::OnNotifyReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	UE_LOG(LogTemp, Warning, TEXT("Notified"));
	
	if (NotifyName == "Attack")
		Attack();
	else if (NotifyName == "Doge")
		CreateAvoidBox();
}

bool AGroundAlien::Attack()
{
	const FVector Forward = GetActorForwardVector();
	//UKismetSystemLibrary::DrawDebugBox(GetWorld(), GetActorLocation() + Forward * m_attackBoxOffset, m_attackBoxSize, FLinearColor::Yellow, FRotator::ZeroRotator, 1, 1);
	
	if (!m_attackTarget) return false;
	return true;
}

bool AGroundAlien::CreateAvoidBox()
{
	const FVector Forward = GetActorForwardVector();
	//UKismetSystemLibrary::DrawDebugBox(GetWorld(), GetActorLocation() + Forward * m_avoidBoxOffset, m_avoidBoxSize, FLinearColor::Blue, FRotator::ZeroRotator, 1.5, 1);

	return true;
}

AlienState AGroundAlien::GetState() const { return m_state; }

void AGroundAlien::SetState(AlienState State) { m_state = State; }

UAnimInstance* AGroundAlien::GetAnimInstance() const
{
	return GetMesh()->GetAnimInstance();
}
