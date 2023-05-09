// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienPlante.h"
#include "GameFramework/Character.h"
#include "AlienPlantSpawner.generated.h"

UCLASS()
class FALLENCORSAIR_API AAlienPlantSpawner : public AAlienPlante
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAlienPlantSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	class UClass* SpawnClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float MaxSpawnHeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int MinSpawnCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int MaxSpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	USoundWave* SoundSpawn;

protected:
	virtual void Spawn();
public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void SpawnActorByClass(UWorld* World, TSubclassOf<class AActor> ClassToSpawn, const FVector& Location);
};
