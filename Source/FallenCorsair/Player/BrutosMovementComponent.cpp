// Fill out your copyright notice in the Description page of Project Settings.


#include "BrutosMovementComponent.h"
#include "FallenCorsair/FallenCorsairCharacter.h"

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
	
}

void UBrutosMovementComponent::DashPressed()
{
	if(m_bCanDash)
	{
		// if(m_bIsSprint)
		// 	MaxWalkSpeed = m_sprintSpeed;
		// else
		// 	MaxWalkSpeed = m_walkSpeed;
	}
}

void UBrutosMovementComponent::PerformDash()
{
	FVector DashDirection = (Acceleration.IsNearlyZero() ? UpdatedComponent->GetForwardVector() : Acceleration.GetSafeNormal2D());
	Velocity = m_impulse * (DashDirection + FVector::UpVector * 0.1f);

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(DashDirection, FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, Hit);
	SetMovementMode(MOVE_Falling);
	
}

void UBrutosMovementComponent::DashCD()
{
	m_bCanDash = true;
}

bool UBrutosMovementComponent::IsCustomMovementMode(EMyCustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}
