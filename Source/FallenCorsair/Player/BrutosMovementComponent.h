// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrutosMovementComponent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum EMyCustomMovementMode
{
	CMOVE_None	UMETA(hidden),
	CMOVE_Dash	UMETA(DisplayName = "Spécial Movement"),
	CMOVE_Max	UMETA(hidden),
};

UCLASS(BlueprintType, Blueprintable)
class FALLENCORSAIR_API UBrutosMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UPROPERTY()
	class AFallenCorsairCharacter* m_ownerRef;

	UPROPERTY()
	bool m_bIsSprint = false;

	/**
	 * Dash
	 */
	UPROPERTY(EditDefaultsOnly)
	float m_dashCD = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float m_impulse = 1000.f;

	UPROPERTY()
	bool m_bCanDash = true;

	FTimerHandle m_dashTimer;
	
public:
	UBrutosMovementComponent();

protected:
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void InitializeComponent() override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	 
public:
	
	UFUNCTION()
	void PerformDash();

	UFUNCTION()
	void DashCD();

	UFUNCTION()
	void DashPressed();
	
	UFUNCTION()
	bool IsCustomMovementMode(EMyCustomMovementMode InCustomMovementMode) const;
};
