// Fill out your copyright notice in the Description page of Project Settings.

#include "AlienInhibitor.h"
#include "Kismet/GameplayStatics.h"
#include "../Waves/WaveSpawner.h"
#include "../Components/Targetable.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAlienInhibitor::AAlienInhibitor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAlienInhibitor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAlienInhibitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAlienInhibitor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AAlienInhibitor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	
	float ActualDamage;

	if (IsAlive())
		if (SoundGetDammaged)
			UGameplayStatics::SpawnSound2D(GetWorld(), SoundGetDammaged);


	ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	if (!IsAlive()){
		DisabledSpawner();

		if (SoundDeath)
		{
			UGameplayStatics::SpawnSound2D(GetWorld(), SoundDeath);
			SoundDeath = nullptr;
		}
	}

    return ActualDamage;
}

void AAlienInhibitor::DisabledSpawner()
{	
	if (!ASpawner)
		return;

	UActorComponent *AC = ASpawner->GetComponentByClass(UWaveSpawner::StaticClass());

	if (!AC)
		return;

	UWaveSpawner *WS = Cast<UWaveSpawner>(AC);

	if (!WS)
		return;
	
	WS->m_bIsActive = false;
}
