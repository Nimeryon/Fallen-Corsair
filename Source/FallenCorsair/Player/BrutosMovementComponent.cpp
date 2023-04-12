// Fill out your copyright notice in the Description page of Project Settings.


#include "BrutosMovementComponent.h"

UBrutosMovementComponent::UBrutosMovementComponent()
{
	
}

void UBrutosMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if(MovementMode == MOVE_Walking)
	{
		if(m_bIsSprint)
			MaxWalkSpeed = m_sprintSpeed;
		else
			MaxWalkSpeed = m_walkSpeed;
	}
}

void UBrutosMovementComponent::SprintPressed()
{
	m_bIsSprint = true;
}

void UBrutosMovementComponent::SprintReleased()
{
	m_bIsSprint = false;
}
