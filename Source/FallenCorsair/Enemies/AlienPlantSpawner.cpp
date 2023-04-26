// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienPlantSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values
AAlienPlantSpawner::AAlienPlantSpawner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAlienPlantSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAlienPlantSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAlienPlantSpawner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called to bind functionality to input
void AAlienPlantSpawner::Spawn()
{
	for(int i = 0; i < ActorToSpawns.Num(); i++)
	{
		FVector OffsetPos = GetOwner()->GetActorForwardVector() * ActorToSpawns[i].OffsetLocationSpawn.X + GetOwner()->GetActorRightVector() * ActorToSpawns[i].OffsetLocationSpawn.Y + GetOwner()->GetActorUpVector() * ActorToSpawns[i].OffsetLocationSpawn.Z;
		FVector SpawnLocation = GetActorLocation() + OffsetPos;
		FRotator SpawnRotation(0.f, 0.f, 0.f);
		FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		if (BPClassToSpawnForAll)
		{
			SpawnActorByClass(GetWorld(), BPClassToSpawnForAll, SpawnLocation, SpawnRotation);
		}
		else 
		{
			SpawnActorByClass(GetWorld(), ActorToSpawns[i].BPClass, SpawnLocation, SpawnRotation);
		}
	}
}

void AAlienPlantSpawner::SpawnActorByClass(UWorld* World, TSubclassOf<AActor> ClassToSpawn, const FVector& Location, const FRotator& Rotation)
{
    if (!World || !ClassToSpawn) return;

    World->SpawnActor<AActor>(ClassToSpawn, Location, Rotation);
}

float AAlienPlantSpawner::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Cast<AAlienBase>(DamageCauser))
		return 0;

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!bIsAlive() && CanEffect)
	{
		CanEffect = false;
		Spawn();
	}

	return ActualDamage;
}