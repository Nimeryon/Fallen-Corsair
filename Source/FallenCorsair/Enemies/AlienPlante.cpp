// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienPlante.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAlienPlante::AAlienPlante()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Créer le composant Static Mesh
    SM_PlanteAlive = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_PlanteAlive"));
    SM_PlanteUnlive = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_PlanteUnlive"));

	SM_PlanteUnlive->SetVisibility(false);

	SM_PlanteAlive->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SM_PlanteUnlive->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called every frame
void AAlienPlante::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsAlive())
	{
		ReviveTimer += DeltaTime;
	}

	if (ReviveTimer >= ReviveCooldown)
	{
		CanEffect = true;
		bCanPlaySoundDestroy = true;
		m_currentHealth = m_health;
		ReviveTimer = 0;
		if (m_reviveSound)
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_reviveSound, GetActorLocation());

		SM_PlanteAlive->SetVisibility(true);
		SM_PlanteUnlive->SetVisibility(false);
	}
}

// Called to bind functionality to input
void AAlienPlante::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AAlienPlante::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	
	float ActualDammage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
	if (!IsAlive() && bCanPlaySoundDestroy)
	{
		bCanPlaySoundDestroy = false;
		SM_PlanteAlive->SetVisibility(false);
		SM_PlanteUnlive->SetVisibility(true);
	}

	return ActualDammage;
}

TArray<FHitResult> AAlienPlante::MakeSphereCollision(float _SphereRadius)
{
	FVector Start = GetActorLocation();
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(_SphereRadius);
	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, FQuat::Identity, UEngineTypes::ConvertToTraceType(ECC_Visibility), SphereShape, QueryParams);

	if (Debug)
	{
		FColor Color = FColor::Red;
		if (OutHits.Num())
			Color = FColor::Green;
		DrawDebugSphere(GetWorld(), Start, _SphereRadius, 10, Color, false, 1, 0, 1);
	}

	return OutHits;
}