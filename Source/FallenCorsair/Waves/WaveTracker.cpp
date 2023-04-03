// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveTracker.h"

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

	UE_LOG(LogTemp, Warning, TEXT("WaveTracker begin"));
	CheckEnemyLeft();
}


// Called every frame
void UWaveTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWaveTracker::CheckEnemyLeft() const
{
	if(m_enemiesAlive <= m_minEnemies)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave Over broadcast"));
		OnWaveOver.Broadcast();
	}
}
