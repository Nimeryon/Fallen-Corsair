// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrutosMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class FALLENCORSAIR_API UBrutosMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float m_sprintSpeed;
	UPROPERTY(EditDefaultsOnly)
	float m_walkSpeed;

	UPROPERTY()
	bool m_bIsSprint = false;
	
public:
	UBrutosMovementComponent();

protected:
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SprintPressed();
	UFUNCTION(BlueprintCallable)
	void SprintReleased();
};
