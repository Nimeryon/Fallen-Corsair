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
	
	UPROPERTY(EditDefaultsOnly)
	float m_dashCD = 1.f;

	UPROPERTY()
	FVector m_startLoc;

	UPROPERTY()
	FVector m_newLoc;

	UPROPERTY()
	float m_groundDistance = 150.f;

	UPROPERTY(EditDefaultsOnly)
	float m_maxDistance = 300.f;

	UPROPERTY()
	float m_actualDistance;

	UPROPERTY(EditDefaultsOnly)
	float m_dashTime = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float m_maxAngleSlope = 45.f;

	UPROPERTY(EditDefaultsOnly)
	float m_invicibleTimePercent = 30.f;

	UPROPERTY(EditDefaultsOnly)
	float m_slowMoTimePercent = 60.f;

	UPROPERTY(EditDefaultsOnly)
	float m_slowMoRate = 10.f;

	UPROPERTY(EditDefaultsOnly)
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
		
};
