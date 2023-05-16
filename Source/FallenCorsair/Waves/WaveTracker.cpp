// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveTracker.h"
#include "FallenCorsair/Enemies/AlienBase.h"

// Sets default values for this component's properties
UWaveTracker::UWaveTracker(): m_enemiesAlive(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWaveTracker::BeginPlay()
{
	Super::BeginPlay();
	
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("WaveTracker begin"));
#endif
	CheckEnemyLeft();

	m_bCanSpawnEnemies = true;
}

// Called every frame
void UWaveTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Enemies alive: %d"), m_enemiesAlive));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Enemies Spawned: %d"), m_enemiesSpawned));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Enemies killed: %d"), m_enemiesKilled));
#endif
}

void UWaveTracker::CheckEnemyLeft() const
{
	if(m_enemiesAlive <= m_minEnemies)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave Over broadcast"));
		OnWaveOver.Broadcast();
	}
}

void UWaveTracker::OnEnemyDeath()
{
	m_enemiesKilled++;
	
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow, FString::Printf(TEXT("enemy died")));
#endif
	if(m_enemiesAlive > 0)
	{
		--m_enemiesAlive;
		CheckEnemyLeft();
	}
}
