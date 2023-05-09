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

// Called when the game starts or when spawned
void AAlienNexus::BeginPlay()
{
	Super::BeginPlay();
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
				UGameplayStatics::SpawnSound2D(GetWorld(), SoundBarrierDisparition);
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

// Called to bind functionality to input
void AAlienNexus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AAlienNexus::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	
	float ActualDamage;

	if (IsAlive())
	{
		if (SoundBarrierHitPlayer)
			UGameplayStatics::SpawnSound2D(GetWorld(), SoundBarrierHitPlayer);
	}

	if (!bAllAliensAreDestroyed())
		DamageAmount *= DammageReductionWhileShield;

	ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!IsAlive() && bCanPlaySoundDeath)
	{
		bCanPlaySoundDeath = false;
		if (SoundNoyauDeath)
			UGameplayStatics::SpawnSound2D(GetWorld(), SoundNoyauDeath);
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


