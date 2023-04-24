// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FallenCorsair/Configs/FallenCorsairSettings.h"
#include "WaveTracker.generated.h"

// Event dispatcher OnWaveOver
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveOver);

UCLASS( ClassGroup=(Wave), meta=(BlueprintSpawnableComponent) )
class FALLENCORSAIR_API UWaveTracker : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWaveTracker();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Check if the number of enemies is under the set minimum
	void CheckEnemyLeft() const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnWaveOver OnWaveOver;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int m_maxEnemies = UConfigUtils::GetMaxAICount();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int m_minEnemies = UConfigUtils::GetMaxAICount()/2;

	UPROPERTY()
	int m_enemiesAlive;
};
