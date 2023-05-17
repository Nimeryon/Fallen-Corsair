// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienNexus.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAlienNexus::AAlienNexus()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshShield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshShield"));
	MeshShield->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called every frame
void AAlienNexus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAllAliensAreDestroyed())
	{
		MeshShield->SetVisibility(false);
		if (bCanPlayBarrierDisparition)
		{
			bCanPlayBarrierDisparition = false;
			if (SoundBarrierDisparition)
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundBarrierDisparition, GetActorLocation());
		}
	}

	if (m_currentHealth < m_health)
	{
		CurrentTimeAutoRegeneration += DeltaTime;
		AutoRegeneration();
	}
	else {
		CurrentTimeAutoRegeneration = 0;
	}
}

float AAlienNexus::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	
	float ActualDamage;

	if (IsAlive())
	{
		if (SoundBarrierHitPlayer)
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundBarrierHitPlayer, GetActorLocation());
	}

	if (!bAllAliensAreDestroyed())
		DamageAmount *= DammageReductionWhileShield;

	ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!IsAlive() && bCanPlaySoundDeath)
	{
		bCanPlaySoundDeath = false;
		if (SoundNoyauDeath)
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundNoyauDeath, GetActorLocation());

		Death();
	}

    return ActualDamage;
}


// Private
void AAlienNexus::AutoRegeneration()
{
	if (CurrentTimeAutoRegeneration >= AutoRegenerationCooldown)
	{
		CurrentTimeAutoRegeneration = 0;
		m_currentHealth += GainHp;

		if (m_currentHealth > m_health)
			m_currentHealth = m_health;
	}
}

bool AAlienNexus::bAllAliensAreDestroyed() const
{
	for (auto *Alien : AliensToDestroyBefore) {
		if (Alien)
		{	
			if (Alien->IsAlive())
			{
				return false;
			}
		}
	}
	return true;
}


