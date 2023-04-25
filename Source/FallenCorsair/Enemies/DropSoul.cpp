// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSoul.h"

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

	soulCollision->OnComponentBeginOverlap.AddDynamic(this, &ADropSoul::OnOverlapBegin);

}

void ADropSoul::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == m_playerRef)
	{
		m_playerRef->barrelComp->CollectSoul();
		Destroy();
	}
}

// Called when the game starts or when spawned
void ADropSoul::BeginPlay()
{
	Super::BeginPlay();
	
	m_playerRef = Cast<AFallenCorsairCharacter> (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ResetLifeSpan();
}

// Called every frame
void ADropSoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	 float CurrentDistance = FVector::Dist(GetActorLocation(), m_playerRef->GetActorLocation());
	
	 FVector Direction  = (m_playerRef->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	
	 if(CurrentDistance < m_distance)
	 {
	 	FVector NewLoc = GetActorLocation() + (Direction * ((m_speed + m_distance - CurrentDistance) * DeltaTime));
	 	SetActorLocation(NewLoc);
	 	ResetLifeSpan();
	}
}

void ADropSoul::ResetLifeSpan()
{
	SetLifeSpan(m_lifeSpan);
}

