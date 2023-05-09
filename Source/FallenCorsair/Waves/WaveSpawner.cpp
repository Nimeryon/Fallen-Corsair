// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawner.h"

#include "VictoryConditionBase.h"
#include "WaveTracker.h"
#include "WaveZone.h"
#include "FallenCorsair/Enemies/AlienBase.h"
#include "FallenCorsair/FallenCorsairGameMode.h"

// Sets default values for this component's properties
UWaveSpawner::UWaveSpawner(): m_spawnTime(0.0f), m_timeToNextSpawn(0.0f), m_bShouldSpawn(true), m_numberOfWaves(0), m_timeActive(0.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner Constructor"));
#endif
}


// Called when the game starts
void UWaveSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	m_player = GetWorld()->GetFirstPlayerController()->GetPawn();
#if WITH_EDITOR
	if(!m_player)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWaveSpawner::BeginPlay - No player"));
	}
#endif
	
	if( m_waveZoneOwner )
	{
		m_waveTracker = m_waveZoneOwner->FindComponentByClass<UWaveTracker>();
		m_zoneVictoryConditions = m_waveZoneOwner->FindComponentByClass<UVictoryConditionBase>();
	}

	if(m_waveTracker)
	{
		m_waveTracker->OnWaveOver.AddDynamic(this, &UWaveSpawner::OnWaveOver);
	}
#if WITH_EDITOR
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveTracker not found"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner begin"));
#endif
	
	m_location = GetOwner()->GetActorLocation();
}


void UWaveSpawner::SpawnEnemy()
{
	if(m_waveTracker && m_waveTracker->m_enemiesAlive < m_waveTracker->m_maxEnemies && m_waveZoneOwner)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;

		const TSubclassOf<AAlienBase> ActorToSpawn = m_waveZoneOwner->GetAlienToSpawn();
		if(ActorToSpawn)
		{
			AAlienBase* SpawnedActor = GetWorld()->SpawnActor<AAlienBase>(ActorToSpawn, GetSpawnLocation(), FRotator::ZeroRotator, SpawnParams);

			if(m_waveTracker)
				SpawnedActor->OnDeath.AddDynamic(m_waveTracker, &UWaveTracker::OnEnemyDeath);

			if(m_zoneVictoryConditions)
				SpawnedActor->OnDeathWithActor.AddDynamic(m_zoneVictoryConditions, &UVictoryConditionBase::OnEnemyDeath);
		}
#if WITH_EDITOR
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UWaveSpawner::SpawnEnemy - No Alien to spawn"));
		}
#endif
	}
#if WITH_EDITOR
	else if (!m_waveZoneOwner)
	{
		UE_LOG(LogTemp, Error, TEXT("UWaveSpawner::SpawnEnemy - WaveZoneOwner not found"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UWaveSpawner::SpawnEnemy - WaveTracker not found"));
	}
#endif
}

FVector3d UWaveSpawner::GetSpawnLocation()
{
	return m_location + FVector3d(FMath::RandRange(-m_spawnDistance, m_spawnDistance), FMath::RandRange(-m_spawnDistance, m_spawnDistance), 0.0f);
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
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Should spawn: %d"), m_bShouldSpawn));
#endif
	// Check if we should spawn
	if(
		m_bIsActive
		&& m_spawnTime > m_timeToNextSpawn
		&& m_bShouldSpawn
		/*&& m_bIsSpawning*/
		&& (m_bSpawnIndefinitely || m_numberOfWaves < m_maxWaves)
		&& m_timeActive > m_timeToFirstSpawn
		&& (!m_bTimedLastSpawn || m_timeActive < m_timeToLastSpawn)
		&& (!m_player || !m_bMaxDistance || m_distanceFromPlayer > FVector::Dist(m_location, m_player->GetActorLocation()))
		&& (!m_bShouldBeInZone || !m_waveZoneOwner || (m_waveZoneOwner->IsPlayerInZone() && m_waveZoneOwner->m_bIsActive))
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

#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Spawning %d enemies"), MinSpawn);
#endif
		for (int i = 0; i < MinSpawn; ++i)
		{
			SpawnEnemy();
			m_waveTracker->m_enemiesAlive++;
		}
		m_bShouldSpawn = (m_waveTracker->m_maxEnemies > m_waveTracker->m_enemiesAlive);
		if(!m_bSpawnIndefinitely)
		{
			++m_numberOfWaves;
		}
	}
	
	
}

void UWaveSpawner::OnWaveOver()
{
	m_bShouldSpawn = true;
}

