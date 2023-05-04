// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienPlante.h"
#include "GameFramework/Character.h"
#include "AlienPlantSpawner.generated.h"

USTRUCT(BlueprintType)
struct FActorToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor To Spawn")
	UClass* BPClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Spawn")
	FVector OffsetLocationSpawn = FVector(0, 0, 0);

};

UCLASS()
class FALLENCORSAIR_API AAlienPlantSpawner : public AAlienPlante
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAlienPlantSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor To Spawn")
	TArray<FActorToSpawn> ActorToSpawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor To Spawn")
	UClass* BPClassToSpawnForAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* SoundSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Spawn();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void SpawnActorByClass(UWorld* World, TSubclassOf<class AActor> ClassToSpawn, const FVector& Location, const FRotator& Rotation);
};
