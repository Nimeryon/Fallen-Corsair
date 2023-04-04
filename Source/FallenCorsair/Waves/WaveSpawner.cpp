// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawner.h"

#include "WaveTracker.h"
#include "WaveZone.h"
#include "FallenCorsair/Enemies/GroundAlien.h"
#include "FallenCorsair/FallenCorsairGameMode.h"

// Sets default values for this component's properties
UWaveSpawner::UWaveSpawner(): m_spawnTime(0.0f), m_timeToNextSpawn(0.0f), m_bShouldSpawn(false), /*m_bIsSpawning(false), */m_numberOfSpawns(0), m_timeActive(0.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner Constructor"));
}


// Called when the game starts
void UWaveSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//get GameMode
	m_waveTracker = Cast<AFallenCorsairGameMode>(GetWorld()->GetAuthGameMode())->FindComponentByClass<UWaveTracker>();

	if(m_waveTracker)
	{
		m_waveTracker->OnWaveOver.AddDynamic(this, &UWaveSpawner::OnWaveOver);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveTracker not found"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner begin"));

	m_location = GetOwner()->GetActorLocation();
}


void UWaveSpawner::SpawnEnemy()
{
	if(m_waveTracker && m_waveTracker->m_enemiesAlive < m_waveTracker->m_maxEnemies)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
		AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(m_enemyToSpawn, m_location, FRotator::ZeroRotator, SpawnParams);
	}
}

// Called every frame
void UWaveSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(m_bIsActive)
	{
		m_timeActive += DeltaTime;
		m_spawnTime += DeltaTime;
	}
	
	// cast to wave zone
	const AWaveZone* waveZone = Cast<AWaveZone>(m_waveZoneOwner);
	
	// Check if we should spawn
	if(
		m_bIsActive
		&& m_spawnTime > m_timeToNextSpawn
		&& m_bShouldSpawn
		/*&& m_bIsSpawning*/
		&& (m_bSpawnIndefinitely || m_numberOfSpawns < m_maxSpawn)
		&& m_timeActive > m_timeToFirstSpawn
		&& m_timeActive < m_timeToLastSpawn
		&& m_distanceFromPlayer > FVector::Dist(m_location, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()) 
	)
	{
		m_spawnTime = 0.0f;
		if(!m_bWaveEffect)
		{
			m_timeToNextSpawn = FMath::RandRange(m_spawnTimeMin, m_spawnTimeMax);
		}

		if(!m_waveTracker)
		{
			UE_LOG(LogTemp, Error, TEXT("WaveTracker not found"));
			return;
		}
		const int MinSpawn = FMath::Min(m_waveTracker->m_maxEnemies - m_waveTracker->m_enemiesAlive, m_numberToSpawn);

		for (int i = 0; i < MinSpawn; ++i)
		{
			SpawnEnemy();
			m_waveTracker->m_enemiesAlive++;
		}
		m_bShouldSpawn = (m_waveTracker->m_maxEnemies > m_waveTracker->m_enemiesAlive);
		if(!m_bSpawnIndefinitely)
		{
			++m_numberOfSpawns;
		}
	}
	
	
}

void UWaveSpawner::OnWaveOver()
{
	m_bShouldSpawn = true;
}

