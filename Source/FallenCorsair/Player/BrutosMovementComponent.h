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

	/**
	 * Dash
	 */
#pragma region Dash Variable
	
	// UPROPERTY()
	// bool m_bCanDash = true;
	//
	// UPROPERTY()
	// bool m_bIsDashing = false;
	//
	// UPROPERTY()
	// bool m_bIsEdge = false;
	//
	// UPROPERTY(EditDefaultsOnly)
	// float m_dashCD = 1.f;
	//
	// UPROPERTY(EditDefaultsOnly)
	// float m_impulse = 1000.f;
	//
	// UPROPERTY(EditDefaultsOnly)
	// FVector m_startLoc;
	//
	// UPROPERTY(EditDefaultsOnly)
	// FVector m_newLoc;
	//
	// UPROPERTY(EditDefaultsOnly)
	// float m_groundDistance = 150.f;
	//
	// UPROPERTY(EditDefaultsOnly)
	// float m_maxDistance = 300.f;
	//
	// UPROPERTY(EditDefaultsOnly)
	// float m_actualDistance;
	//
	// UPROPERTY(EditDefaultsOnly)
	// float m_dashTime = 0.5f;
	//
	// UPROPERTY(EditDefaultsOnly)
	// float m_maxAngleSlope = 45.f;
	//
	// UPROPERTY(EditDefaultsOnly)
	// float m_invicibleTimePercent = 30.f;
	//
	// UPROPERTY(EditDefaultsOnly)
	// float m_slowMoRate = 10.f;
	//
	// FTimerHandle m_dashTimer;

#pragma endregion 
	
public:
	UBrutosMovementComponent();

protected:

	
	
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void InitializeComponent() override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	 
public:
	
	// UFUNCTION()
	// void PerformDash();
	//
	// UFUNCTION()
	// void StartDash();
	//
	// UFUNCTION()
	// void StopDash();
	//
	// UFUNCTION()
	// void DetectEdge();
	//
	// UFUNCTION()
	// void DashCD();
	//
	// UFUNCTION()
	// void DashPressed();
	//
	// UFUNCTION()
	// void PhysDash(float deltaTime);
	//
	// UFUNCTION()
	// bool IsCustomMovementMode(EMyCustomMovementMode InCustomMovementMode) const;
};
