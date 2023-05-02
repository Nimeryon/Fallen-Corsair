// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienPlantBomb.h"
#include "AlienBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/Explosion.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "../FallenCorsairCharacter.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AAlienPlantBomb::AAlienPlantBomb()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAlienPlantBomb::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAlienPlantBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsAlive() && !bDetectedSomeone)
	{
		DetectPlayer();
	}

	if (!bDetectedSomeone)
		return;

	CurrentCountdown += DeltaTime;

	if (CurrentCountdown >= CountdownBeforeExplosion)
	{
		bDetectedSomeone = false;
		CurrentCountdown = 0;
		m_currentHealth = 0;

		if (!bIsAlive() && CanEffect)
		{
			CanEffect = false;
			if (SoundExplosion)
				UGameplayStatics::SpawnSound2D(GetWorld(), SoundExplosion);

			if (AudioComponentDetonator)
			{
				AudioComponentDetonator->Stop();
				AudioComponentDetonator->DestroyComponent();
				AudioComponentDetonator = nullptr;
			}

			UExplosion::PerformExplosion(GetWorld(), GetOwner(), Dammage, GetOwner()->GetActorLocation(), SphereRadius, PropulsionForce, RotationAngleDegrees, NS_Explosion, Debug);
		}
	}
}

// Called to bind functionality to input
void AAlienPlantBomb::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AAlienPlantBomb::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	
	AAlienBase* Alien = Cast<AAlienBase>(DamageCauser);
	
	if (Alien)
		return 0;

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!bIsAlive() && CanEffect)
	{
		CanEffect = false;

		if (SoundExplosion)
			UGameplayStatics::SpawnSound2D(GetWorld(), SoundExplosion);

		if (AudioComponentDetonator)
		{
			AudioComponentDetonator->Stop();
			AudioComponentDetonator->DestroyComponent();
			AudioComponentDetonator = nullptr;
		}

		UExplosion::PerformExplosion(GetWorld(), GetOwner(), Dammage, GetOwner()->GetActorLocation(), SphereRadius, PropulsionForce, RotationAngleDegrees, NS_Explosion, Debug);
	}

    return ActualDamage;
}

void AAlienPlantBomb::DetectPlayer() {
	if (bDetectedSomeone)
		return;

	TArray<FHitResult> OutHits = MakeSphereCollision(DetectionSphereRadius);

	for (auto It = OutHits.CreateIterator(); It; It++)
	{
		AFallenCorsairCharacter *FallenCorsairCharacter = Cast<AFallenCorsairCharacter>((*It).GetActor());
		if (FallenCorsairCharacter)
		{
			bDetectedSomeone = true;

			if (SoundDetonator)
			{
				if (!AudioComponentDetonator)
				{
					AudioComponentDetonator = UGameplayStatics::SpawnSound2D(GetWorld(), SoundDetonator);
					if (AudioComponentDetonator)
					{
						AudioComponentDetonator->bAutoDestroy = false;
						AudioComponentDetonator->bStopWhenOwnerDestroyed = false;
						SoundDetonator->bLooping = true;
					}
				}
			}
		}
	}
}



