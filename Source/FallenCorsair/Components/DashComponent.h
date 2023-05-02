// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLENCORSAIR_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDashComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY()
	class AFallenCorsairCharacter* m_ownerRef;

	UPROPERTY()
	class UBarrel* m_barrelRef;

#pragma region Dash Variable
	
	UPROPERTY()
	bool m_bCanDash = true;

	UPROPERTY()
	bool m_bIsPerfectDodge = false;

	UPROPERTY()
	bool m_bIsDashing = false;

	UPROPERTY()
	bool m_bIsEdge = false;

	UPROPERTY()
	bool m_bIsSlowMo = false;

	UPROPERTY()
	FVector m_startLoc;

	UPROPERTY()
	FVector m_newLoc;

	UPROPERTY()
	bool m_bHasIFrames = true;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (displayName = "Dash Cool Down"), meta = (ClampMin = 0.2, UIMin = 0.2, ClampMax = 10, UIMax = 10))
	float m_dashCD = 1.f;

	UPROPERTY()
	float m_groundDistance = 200.f;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (displayName = "Distance max du dash"), meta = (ClampMin = 100, UIMin = 100, ClampMax = 3000, UIMax = 3000))
	float m_maxDistance = 300.f;

	UPROPERTY()
	float m_actualDistance;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (displayName = "Durée du dash"), meta = (ClampMin = 0.2, UIMin = 0.2, ClampMax = 10, UIMax = 10))
	float m_dashTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (displayName = "Dash Angle max des pentes"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 60, UIMax = 60))
	float m_maxAngleSlope = 45.f;

	/**
	 * The Player Is Invincible at the start of the dash until the percentage of time given
	 */
	UPROPERTY(EditAnywhere, Category = "Dash", meta = (displayName = "Moment d'invincibilité"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 100, UIMax = 100))
	float m_invicibleTimePercent = 30.f;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (displayName = "Coût du Dash"), meta = (ClampMin = 0, UIMin = 0))
	float m_energyCost = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Dash", meta = (displayName = "Dash Curve"))
	UCurveFloat* m_DashCurve;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (displayName = "Dash Impulse On Ennemy"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 10000, UIMax = 10000))
	float m_impulse = 500.f;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (displayName = "Dash Impulse Z On Ennemy"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 10000, UIMax = 10000))
	float m_impulseZ = 0.f;

	/**
	 * The Player Launch The SlowMo at the given percentage of time of the dash
	 */
	UPROPERTY(EditAnywhere, Category = "Dash|SlowMo", meta = (displayName = "Moment du SlowMo"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 100, UIMax = 100))
	float m_slowMoTimePercent = 60.f;

	UPROPERTY(EditAnywhere, Category = "Dash|SlowMo", meta = (displayName = "Puissance du SlowMo"), meta = (ClampMin = 1, UIMin = 1, ClampMax = 30, UIMax = 30))
	float m_slowMoRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "Dash|SlowMo", meta = (displayName = "Durée du SlowMo"), meta = (ClampMin = 0.2, UIMin = 0.2, ClampMax = 10, UIMax = 10))
	float m_slowMoDuration = 1.f;
	
	FTimerHandle m_dashTimer;
	FTimerHandle m_slowMoTimer;

#pragma endregion

public:

UFUNCTION()
	void DashPressed();
	
	UFUNCTION()
	void PerformDash();

	UFUNCTION()
	void StartDash();

	UFUNCTION()
	void StopDash();
	
	UFUNCTION()
	void DetectEdge();

	UFUNCTION()
	void DashCD();

	UFUNCTION()
	void DashSlowMoReset();

	UFUNCTION()
	void PerfectDodge();

	UFUNCTION()
	void ImpulseEnnemy();
		
};
