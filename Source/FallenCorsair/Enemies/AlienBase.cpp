// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAlienBase::AAlienBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAlienBase::BeginPlay()
{
	Super::BeginPlay();

	m_movementSpeed = UKismetMathLibrary::RandomFloatInRange(m_minMovementSpeed, m_maxMovementSpeed);
	GetCharacterMovement()->MaxWalkSpeed = m_movementSpeed;
	m_currentHealth = m_health;

	if (OnSpawn.IsBound())
		OnSpawn.Broadcast();
}

// Called every frame
void AAlienBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bStunned && m_bCanBeStunned)
	{
		m_currentStunTime += DeltaTime;
		if (m_currentStunTime >= m_stunTime)
		{
			m_bStunned = false;
			m_currentStunTime = 0;
			Unstunned();
		}
	}
}

// Called to bind functionality to input
void AAlienBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AAlienBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
	if (!m_bCanTakeDamage)
		return Super::TakeDamage(0, DamageEvent, EventInstigator, DamageCauser);
	
	// Calculate damage based on DamageEvent
	const FDamageTypeEvent* damage = static_cast<const FDamageTypeEvent*>(&DamageEvent);
	EDamageType damageType = EDamageType::Default;
	if (damage)
		damageType = damage->DamageType;
	DamageAmount *= GetDamageMultiplicator(damageType);
	
	// Apply damage
	if (OnDamaged.IsBound())
		OnDamaged.Broadcast(DamageAmount);
	
	m_currentHealth = FMath::Clamp(m_currentHealth - DamageAmount, 0.f, m_health);
	UE_LOG(LogTemp, Warning, TEXT("%d"), m_currentHealth);

	if (m_currentHealth <= 0)
		Death(damageType);

	if(m_hurtParticle)
	{
		FVector SpawnLocation = GetActorLocation();
		FVector Scale = FVector(1, 1, 1);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		// Spawn the Niagara FX system at the specified location and rotation
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_hurtParticle, SpawnLocation, SpawnRotation,  Scale, true);	
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

bool AAlienBase::IsAlive() const
{
	return m_currentHealth > 0;
}

bool AAlienBase::IsStunned() const
{
	return m_bStunned;
}

float AAlienBase::GetRemainingStunTime() const
{
	return m_stunTime - m_currentStunTime;
}

float AAlienBase::GetStunTime() const
{
	return  m_stunTime;
}

bool AAlienBase::Stun(float Time)
{
	if (!m_bCanBeStunned) return false;

	if (OnStunned.IsBound())
		OnStunned.Broadcast(Time);

	m_bStunned = true;
	m_stunTime = Time;
	m_currentStunTime = 0.f;
	return true;
}

void AAlienBase::Unstunned() {}

float AAlienBase::GetDamageMultiplicator(EDamageType DamageType) const
{
	switch (DamageType)
	{
	case EDamageType::MeleeSoft:
		return m_attackMeleeSoftMultiplicator;
	case EDamageType::MeleeHeavy:
		return m_attackMeleeHeavyMultiplicator;
	case EDamageType::Distance:
		return m_attackDistanceMultiplicator;
	case EDamageType::Explosion:
		return m_attackExplosionMultiplicator;
	default: return 1.f;
	}
}

void AAlienBase::Death(EDamageType DamageType)
{
	if (OnDeath.IsBound())
		OnDeath.Broadcast();

	if (OnDeathWithActor.IsBound())
		OnDeathWithActor.Broadcast(this);
		
	if (m_destroyOnDeath)
	{
		if(m_deathParticle)
		{
			FVector SpawnLocation = GetActorLocation();
			FVector Scale = FVector(1, 1, 1);
			FRotator SpawnRotation = FRotator::ZeroRotator;
			// Spawn the Niagara FX system at the specified location and rotation
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_deathParticle, SpawnLocation, SpawnRotation,  Scale, true);	
		}
		Destroy();
	}
}
