// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveZone.h"

#include "WaveTracker.h"
#include "Components/BoxComponent.h"
#include "FallenCorsair/FallenCorsairCharacter.h"

// Sets default values
AWaveZone::AWaveZone(): m_player(nullptr), m_bIsPlayerInZone(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_waveTracker = CreateDefaultSubobject<UWaveTracker>(TEXT("WaveTracker"));
}

// Called when the game starts or when spawned
void AWaveZone::BeginPlay()
{
	Super::BeginPlay();

	m_player = GetWorld()->GetFirstPlayerController()->GetPawn();

	m_collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWaveZone::OnOverlapBegin);
	m_collisionBox->OnComponentEndOverlap.AddDynamic(this, &AWaveZone::OnOverlapEnd);

	m_bIsPlayerInZone = m_collisionBox->IsOverlappingActor(m_player);
	if(m_bIsPlayerInZone)
	{
		UE_LOG(LogTemp, Display, TEXT("Player in zone"));
	}
	else
	{
		Cast<AFallenCorsairCharacter>(m_player)->OnPlayerSpawn.AddDynamic(this, &AWaveZone::OnPlayerSpawn);
	}
}

// Called every frame
void AWaveZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaveZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == m_player)
	{
		UE_LOG(LogTemp, Display, TEXT("Player in zone"));
		m_bIsPlayerInZone = true;
	}
}

void AWaveZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == m_player)
	{
		UE_LOG(LogTemp, Display, TEXT("Player no longer in zone"));
		m_bIsPlayerInZone = false;
	}
}

bool AWaveZone::IsPlayerInZone() const
{
	return m_bIsPlayerInZone;
}

void AWaveZone::OnPlayerSpawn()
{
	m_bIsPlayerInZone = m_collisionBox->IsOverlappingActor(m_player);
}

TSubclassOf<AGroundAlien> AWaveZone::GetAlienToSpawn() const
{
	float totalRatio = 0.f;

	for (const FEnemies& enemy : m_enemies)
	{
		totalRatio += enemy.Ratio;
	}
	
	float random = FMath::FRandRange(0.f, 1.f) * totalRatio;
	
	float ratio = 0.f;
	for (const FEnemies& enemy : m_enemies)
	{
		ratio += enemy.Ratio;
		if (random <= ratio)
		{
			return enemy.Enemy;
		}
	}
	return nullptr;
}
