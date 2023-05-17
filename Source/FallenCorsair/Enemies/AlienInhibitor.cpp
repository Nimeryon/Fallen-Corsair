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
	PrimaryActorTick.bCanEverTick = false;
}

float AAlienInhibitor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (!IsAlive())
		DisabledSpawner();

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

void AAlienInhibitor::Death(EDamageType DamageType)
{
	PlayDeathFX();
	if (SplineMesh)
		SplineMesh->Destroy();

	Super::Death(DamageType);
}

