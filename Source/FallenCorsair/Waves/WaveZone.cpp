// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveZone.h"

#include "Components/BoxComponent.h"

// Sets default values
AWaveZone::AWaveZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWaveZone::BeginPlay()
{
	Super::BeginPlay();

	m_collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWaveZone::OnOverlapBegin);
	m_collisionBox->OnComponentEndOverlap.AddDynamic(this, &AWaveZone::OnOverlapEnd);
}

// Called every frame
void AWaveZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaveZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// get player actor
	const AActor* player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (OtherActor == player)
	{
		m_bIsPlayerInZone = true;
		
	}
}

void AWaveZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// get player actor
	const AActor* player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (OtherActor == player)
	{
		m_bIsPlayerInZone = false;
	}
}

bool AWaveZone::IsPlayerInZone() const
{
	return m_bIsPlayerInZone;
}

