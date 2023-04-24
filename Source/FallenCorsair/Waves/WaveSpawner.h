// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaveZone.h"
#include "Components/ActorComponent.h"
#include "WaveSpawner.generated.h"


UCLASS( ClassGroup=(Wave), meta=(BlueprintSpawnableComponent) )
class FALLENCORSAIR_API UWaveSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWaveSpawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void SpawnEnemy();

	UFUNCTION()
	virtual FVector3d GetSpawnLocation();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the wave is over and we can spawn more enemies
	UFUNCTION()
	virtual void OnWaveOver();

protected:
	// The maximum distance from the spawner to spawn the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (Units="cm"))
	float m_spawnDistance = 10.0f;

	// The zone to which the spawner is attached
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wave")
	AWaveZone* m_waveZoneOwner;
	
	UPROPERTY(BlueprintReadWrite, Category = "Wave")
	class UWaveTracker* m_waveTracker;

	UPROPERTY()
	APawn* m_player;
	
	// Check if we want no delay between spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	bool m_bWaveEffect = false;

	// The minimum time to wait between spawns, not editable if wave effect is activated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave", meta = (EditCondition="!m_bWaveEffect", Units="s"))
	float m_spawnTimeMin = 1.0f;

	// The maximum time to wait between spawns, not editable if wave effect is activated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave", meta = (EditCondition="!m_bWaveEffect", Units="s"))
	float m_spawnTimeMax = 2.0f;

	// Track the time between spawns
	UPROPERTY()
	float m_spawnTime;

	// Store the time to next spawn
	UPROPERTY()
	float m_timeToNextSpawn;

	// Track if the spawner should spawn
	UPROPERTY()
	bool m_bShouldSpawn;

	// Set to false to disable the spawner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool m_bIsActive = true;

	// Set to false to have a set number of waves
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Conditions|NumberOfWaves")
	bool m_bSpawnIndefinitely = true;

	// Track the number of waves spawned
	UPROPERTY(VisibleAnywhere, Category = "Spawn|Conditions|NumberOfWaves")
	int m_numberOfWaves;

	// The maximum number of waves to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Conditions|NumberOfWaves", meta = (EditCondition="!m_bSpawnIndefinitely"))
	int m_maxWaves = 0;

	// Number of enemies spawned per wave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Enemies")
	int m_numberToSpawn = 1;

	// Track the amount of time the spawner has been active
	UPROPERTY()
	float m_timeActive;
	
	// The time to wait before the spawner starts spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Conditions|Timers", meta = (Units = "s"))
	float m_timeToFirstSpawn = 0.0f;

	// Set to true if we want to deactivate the spawner after a set time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Conditions|Timers")
	bool m_bTimedLastSpawn = false;

	// The time to wait before the spawner stops spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Conditions|Timers", meta = (EditCondition="m_bTimedLastSpawn", Units = "s"))
	float m_timeToLastSpawn = 180.0f;
	
	// Set to true if we want to deactivate the spawner if the player is too far away
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Conditions|Distance")
	bool m_bMaxDistance = false;
	
	// The distance the player needs to be from the spawner to spawn enemies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Conditions|Distance", meta = (EditCondition="m_bMaxDistance", Units = "cm"))
	float m_distanceFromPlayer = 1000.0f;
	
	// Set to true if we want to deactivate the spawner if the player is not in the parent zone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Conditions|Distance")
	bool m_bShouldBeInZone = true;

	// The world location of the spawner
	UPROPERTY()
	FVector3d m_location;
};
