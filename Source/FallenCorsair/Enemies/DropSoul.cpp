// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSoul.h"

#include <msctf.h>

#include "../FallenCorsairCharacter.h"
#include "Components/SphereComponent.h"
#include "FallenCorsair/Components/Barrel.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADropSoul::ADropSoul()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	soulCollision = CreateDefaultSubobject<USphereComponent>("Soul Collision");
	RootComponent = soulCollision;

	soulMesh = CreateDefaultSubobject<UStaticMeshComponent>("Soul Mesh");
	soulMesh->SetupAttachment(soulCollision);
}

// Called when the game starts or when spawned
void ADropSoul::BeginPlay()
{
	Super::BeginPlay();
	
	m_playerRef = Cast<AFallenCorsairCharacter> (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ADropSoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_bCanBePickedUp)
	{
		m_currentPickupTime += DeltaTime;
		if (m_currentPickupTime >= m_pickupTime)
		{
			m_bCanBePickedUp = true;
			ResetLifeSpan();
		}
	}
	else
	{
		const float CurrentDistance = FVector::Dist(GetActorLocation(), m_playerRef->GetActorLocation());
		const FVector Direction  = (m_playerRef->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		if(CurrentDistance < m_distance)
		{
			const FVector NewLoc = GetActorLocation() + (Direction * ((m_speed + m_distance - CurrentDistance) * DeltaTime));
			SetActorLocation(NewLoc);
			ResetLifeSpan();
		}

		if (!m_bPickedUp && CurrentDistance <= m_pickupDistance)
		{
			m_playerRef->barrelComp->CollectSoul();
			m_bPickedUp = true;
		}
	}

	if (m_bPickedUp)
	{
		m_currentDestroyTime += DeltaTime;
		if (m_currentDestroyTime >= m_destroyTime)
			Destroy();
	}
}

void ADropSoul::ResetLifeSpan()
{
	SetLifeSpan(m_lifeSpan);
}

