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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float m_spawnDistance = 10.0f;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	// TSubclassOf<class AGroundAlien> m_enemyToSpawn;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawn")
	AWaveZone* m_waveZoneOwner;
	
	UPROPERTY()
	class UWaveTracker* m_waveTracker;
	
	// Check if we want no delay between spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	bool m_bWaveEffect = false;

	// The minimum time to wait between spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave", meta = (EditCondition="m_bWaveEffect"))
	float m_spawnTimeMin = 1.0f;

	// The maximum time to wait between spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave", meta = (EditCondition="m_bWaveEffect"))
	float m_spawnTimeMax = 2.0f;

	// Track the time between spawns
	UPROPERTY()
	float m_spawnTime;

	// Store the time to next spawn
	UPROPERTY()
	float m_timeToNextSpawn;

	// Set to true if the spawner should spawn
	UPROPERTY()
	bool m_bShouldSpawn;

	/*
	// Set to true if the spawner is currently spawning, disables spawning until the current spawn is finished
	UPROPERTY()
	bool m_bIsSpawning;
	*/

	// Set to false to disable the spawner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool m_bIsActive = true;

	// Set to false to have a set number of waves
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool m_bSpawnIndefinitely = true;

	// Track the number of waves spawned
	UPROPERTY(VisibleAnywhere, Category = "Spawn", meta = (EditCondition="!m_bSpawnIndefinitely"))
	int m_numberOfSpawns;

	// The maximum number of waves to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (EditCondition="!m_bSpawnIndefinitely"))
	int m_maxSpawn = 0;

	// Number of enemies spawned per wave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int m_numberToSpawn = 1;

	// Track the amount of time the spawner has been active
	UPROPERTY()
	float m_timeActive;
	
	// The time to wait before the spawner starts spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float m_timeToFirstSpawn = 0.0f;

	// The time to wait before the spawner stops spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float m_timeToLastSpawn = 180.0f;

	// The distance the player needs to be from the spawner to spawn enemies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float m_distanceFromPlayer = 1000.0f;

	// The world location of the spawner
	UPROPERTY()
	FVector3d m_location;
};
