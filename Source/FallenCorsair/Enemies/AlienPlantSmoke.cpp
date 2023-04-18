// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienPlantSmoke.h"
#include "AlienBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
AAlienPlantSmoke::AAlienPlantSmoke()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAlienPlantSmoke::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAlienPlantSmoke::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAlienPlantSmoke::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AAlienPlantSmoke::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Cast<AAlienBase>(DamageCauser))
		return 0;

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!bIsAlive() && CanEffect)
	{
		CanEffect = false;
		StunAlien(); // Stun all aliens at proximity
	}

	return ActualDamage;
}

void AAlienPlantSmoke::StunAlien()
{
	// Make explosion smoke that will stun aliens hited
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereRadius);

	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	FVector Origin = GetActorLocation();
	GetWorld()->SweepMultiByObjectType(OutHits, Origin, Origin, FQuat::Identity, UEngineTypes::ConvertToTraceType(ECC_Visibility), SphereShape, QueryParams);

	for (auto It = OutHits.CreateIterator(); It; It++)
	{
		AAlienBase* Alien = Cast<AAlienBase>((*It).GetActor());

		if (Alien)
		{
			Alien->Stun(StunDuration);
		}
	}

	// Niagara Smoke
	if (NS_Smoke)
	{
		// Check if the Niagara FX asset was loaded successfully
		// Set up the spawn parameters
		FVector SpawnLocation = GetActorLocation();
		FVector Scale = FVector(1, 1, 1);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		// Spawn the Niagara FX system at the specified location and rotation
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Smoke, SpawnLocation, SpawnRotation, Scale, true);
	}

	if (Debug)
	{
		FColor Color = FColor::Red;
		if (OutHits.Num())
			Color = FColor::Green;
		DrawDebugSphere(GetWorld(), Origin, SphereRadius, 10, Color, false, 1, 0, 1);
	}
}

