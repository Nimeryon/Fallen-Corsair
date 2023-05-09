// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VictoryConditionBase.generated.h"


// Event dispatcher OnZoneVictory
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZoneVictory);

UENUM()
enum class EConditionType
{
	EnemiesKilled,
	EnemiesRemaining,
	WavesCleared,
	//Timer,
	ActionPerformed,
};

UENUM()
enum class EActionType
{
	LightAttack,
	HeavyAttack,
	Shoot,
	Dodge,
};

USTRUCT()
struct FVictoryCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EConditionType ConditionType;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "ConditionType == EConditionType::EnemiesKilled", EditConditionHides))
	TSubclassOf<class AAlienBase> AlienType;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ConditionType == EConditionType::ActionPerformed", EditConditionHides))
	EActionType ActionPerformed;

	UPROPERTY(VisibleAnywhere, meta = (EditCondition = "ConditionType != EConditionType::EnemiesRemaining"))
	int32 Count=0;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ConditionType != EConditionType::EnemiesRemaining"))
	int32 Goal=1;

	UPROPERTY(EditAnywhere)
	bool bIsConditionMet;
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLENCORSAIR_API UVictoryConditionBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVictoryConditionBase();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Victory Condition")
	FZoneVictory OnZoneVictory;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Victory Condition")
	TArray<FVictoryCondition> m_victoryConditions;

	UPROPERTY()
	class UWaveTracker* m_waveTracker;

	UPROPERTY(VisibleAnywhere)
	bool m_bAllConditionsMet = false;

	UPROPERTY()
	bool m_killAll = false;
	
public:
	UFUNCTION()
	virtual void CheckConditions();

	UFUNCTION()
	virtual void OnLightAttackPerformed();

	UFUNCTION()
	virtual void OnHeavyAttackPerformed();

	UFUNCTION()
	virtual void OnShootPerformed();

	UFUNCTION()
	virtual void OnDodgePerformed();

	UFUNCTION()
	virtual void OnWaveOver();

	UFUNCTION(BlueprintCallable)
	virtual void OnEnemyDeath(AAlienBase* Alien);
};
