// Fill out your copyright notice in the Description page of Project Settings.


#include "BrutosMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "FallenCorsair/FallenCorsairCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

UBrutosMovementComponent::UBrutosMovementComponent()
{
}

void UBrutosMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	/// tick
	
}

void UBrutosMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	m_ownerRef = Cast<AFallenCorsairCharacter>(GetOwner());
}

void UBrutosMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Dash:
		PhysDash(deltaTime, Iterations);
		break;
	default:
			UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement"))
	}
	
}

void UBrutosMovementComponent::DashPressed()
{
	if(m_bCanDash)
	{
		PerformDash();
		m_bCanDash = false;
		GetWorld()->GetTimerManager().SetTimer(m_dashTimer, this, &UBrutosMovementComponent::DashCD, m_dashCD);
	}
}

void UBrutosMovementComponent::PhysDash(float deltaTime, int32 Iterations)
{
	if(m_bIsEdge)
	{
		DetectEdge();
		if(m_bIsDashing)
		{
			float CurrentDistance = (m_ownerRef->GetActorLocation() - m_startLoc).Length() + 5;
			if(CurrentDistance <= m_actualDistance)
			{
				MaxWalkSpeed = m_maxDistance / m_dashTime;
				Velocity = (m_newLoc - m_startLoc).GetSafeNormal() * MaxWalkSpeed;
				AddInputVector((m_newLoc - m_startLoc).GetSafeNormal(), true);
				
				FHitResult Hit(1.f);
				//SafeMoveUpdatedComponent(Velocity * deltaTime, m_ownerRef->GetActorRotation(), true, Hit);
				
				UE_LOG(LogTemp, Warning, TEXT("Dash"));
			}
			else
			{
				StopDash();
				StartNewPhysics(deltaTime, Iterations);
			}
		}
	}
}

void UBrutosMovementComponent::PerformDash()
{
	// FVector DashDirection = (Acceleration.IsNearlyZero() ? UpdatedComponent->GetForwardVector() : Acceleration.GetSafeNormal2D());
	// Velocity = m_impulse * (DashDirection + FVector::UpVector * 0.1f);
	//
	// FQuat NewRotation = FRotationMatrix::MakeFromXZ(DashDirection, FVector::UpVector).ToQuat();
	// FHitResult Hit;
	// SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, Hit);
	// SetMovementMode(MOVE_Falling);

	SetMovementMode(MOVE_Custom, CMOVE_Dash);
	m_startLoc = m_ownerRef->GetActorLocation();
	m_newLoc = m_startLoc + (m_ownerRef->GetActorForwardVector() * m_maxDistance);
	const float CapsuleRadius = m_ownerRef->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const float CapsuleHalfHeight = m_ownerRef->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FHitResult Hit;

	UKismetSystemLibrary::CapsuleTraceSingleByProfile(GetWorld(), m_startLoc, m_newLoc, CapsuleRadius, CapsuleHalfHeight, TEXT("Visibility"), false, m_ownerRef->SetIgnoreCharacterActors(), EDrawDebugTrace::ForDuration, Hit, true);

	if(Hit.bBlockingHit)
	{
		if(FVector::DotProduct(Hit.Normal, (m_startLoc - Hit.Location).GetSafeNormal()) > 90 / m_maxAngleSlope)
			m_newLoc = Hit.Location;
		else
			m_newLoc = Hit.TraceEnd;
	}
	else
	{
		m_newLoc = Hit.TraceEnd;
	}

	m_bIsEdge = true;
}

void UBrutosMovementComponent::StartDash()
{
	m_actualDistance = (m_newLoc - m_startLoc).Length();
	m_bIsDashing = true;

	FVector startTrace = m_ownerRef->GetActorLocation();
	FVector endTrace = startTrace + (m_ownerRef->GetActorForwardVector() * 50);
	FHitResult Hit;

	
	UKismetSystemLibrary::SphereTraceSingleByProfile(GetWorld(), startTrace, endTrace, 30.f, TEXT("Visibility"), false, m_ownerRef->SetIgnoreCharacterActors(), EDrawDebugTrace::ForDuration, Hit, true);

	if(Hit.bBlockingHit && FVector::DotProduct(Hit.Normal, (Hit.TraceStart - Hit.Location).GetSafeNormal()) < 1)
	{
		StopDash();
	}
	
}

void UBrutosMovementComponent::StopDash()
{
	SetMovementMode(MOVE_Walking);
	m_bIsDashing = false;
	m_bIsEdge = false;
	MaxWalkSpeed = 500;
	Velocity = FVector(0,0,0);
	StopMovementImmediately();
	
	
}

void UBrutosMovementComponent::DetectEdge()
{

	FVector startTrace = m_ownerRef->GetActorLocation() + ((m_newLoc - m_ownerRef->GetActorLocation()).GetSafeNormal() * 50);
	FVector endTrace  =( m_ownerRef->GetActorLocation() + (m_ownerRef->GetActorForwardVector() * 100)) - FVector(0,0, m_groundDistance);
	FHitResult Hit;

	UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), startTrace, endTrace, TEXT("Visibility"), false,  m_ownerRef->SetIgnoreCharacterActors(), EDrawDebugTrace::ForDuration, Hit, true);

	if(Hit.bBlockingHit)
		StartDash();
	else
		StopDash();
	
}

void UBrutosMovementComponent::DashCD()
{
	m_bCanDash = true;
}

bool UBrutosMovementComponent::IsCustomMovementMode(EMyCustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}
