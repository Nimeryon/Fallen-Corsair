// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienPlantSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAlienPlantSpawner::AAlienPlantSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called to bind functionality to input
void AAlienPlantSpawner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called to bind functionality to input
void AAlienPlantSpawner::Spawn()
{
	const int SpawnCount = UKismetMathLibrary::RandomIntegerInRange(MinSpawnCount, MaxSpawnCount);
	for (int i = 0; i < SpawnCount; ++i)
	{
		const FVector SpawnOffset = FVector(
			FMath::Cos(UKismetMathLibrary::RandomFloat()) * SpawnRadius,
			FMath::Sin(UKismetMathLibrary::RandomFloat()) * SpawnRadius,
			UKismetMathLibrary::RandomFloatInRange(GetActorLocation().Z, GetActorLocation().Z + MaxSpawnHeight)
		);
		const FVector SpawnLocation = GetActorLocation() + SpawnOffset;

		SpawnActorByClass(GetWorld(), SpawnClass, SpawnLocation);
	}
	
	if (SoundSpawn)
		UGameplayStatics::SpawnSound2D(GetWorld(), SoundSpawn);
}

void AAlienPlantSpawner::SpawnActorByClass(UWorld* World, TSubclassOf<AActor> ClassToSpawn, const FVector& Location)
{
    if (!World || !ClassToSpawn) return;

    World->SpawnActor<AActor>(ClassToSpawn, Location, FRotator::ZeroRotator);
}

float AAlienPlantSpawner::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Cast<AAlienBase>(DamageCauser))
		return 0;

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!IsAlive() && CanEffect)
	{
		CanEffect = false;
		Spawn();
	}

	return ActualDamage;
}