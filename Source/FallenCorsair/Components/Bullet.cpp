// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "FallenCorsair/Enemies/AlienBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../FallenCorsairCharacter.h"
#include "Kismet/KismetStringLibrary.h"
#include "FallenCorsair/Enemies/AlienBase.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bulletCollision = CreateDefaultSubobject<USphereComponent>("Bullet Collision");
	RootComponent = bulletCollision;

	bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("Bullet Mesh");
	bulletMesh->SetupAttachment(bulletCollision);

	dammageCollision = CreateDefaultSubobject<USphereComponent>("Dammage Collision");
	dammageCollision->SetupAttachment(bulletCollision);
	
	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	projectileMovement->MaxSpeed = 20000.f;
	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Timer;
	//GetWorld()->GetTimerManager().SetTimer(Timer, this, &ABullet::Explosion, m_lifeSpan);

	// Bind to OnAudioFinished delegate
	if (SoundChargeComplete)
	{
		AudioComponentChargeComplete = NewObject<UAudioComponent>(this);
		AudioComponentChargeComplete->SetSound(SoundChargeComplete);
		// Jouer le son en boucle
		AudioComponentChargeComplete->bAutoActivate = false; // Désactiver la lecture automatique pour éviter un délai
		AudioComponentChargeComplete->bStopWhenOwnerDestroyed = false; // Empêcher le composant audio d'être détruit avec le propriétaire
		SoundChargeComplete->bLooping = true; // Activer la lecture en boucle
	}
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#pragma region Bullet Charge

	if (bLoopStartedSoundChargeCompleted)
	{
		PlayChargeCompletedSound();
	}

	if(!m_bIsBulletLaunch && !Explosed)
	{
		SetActorLocation(m_ownerRef->GetMesh()->GetSocketLocation("BulletStart"));
		SetActorRotation(m_ownerRef->GetFollowCamera()->GetComponentRotation());
		PlayChargeSound();
	}

	
	if(m_bIsCharging && m_ownerRef)
	{
		if(m_currentCharge >= 1)
		{
			m_bIsFullyCharge = true;
			m_bIsCharging = false;
			StopAudioComponent(AudioComponentCharge);
			PlayChargeCompletedSound();
		}
		
		FMath::Clamp(m_currentCharge = m_currentCharge + 1 / m_chargeSpeed * DeltaTime,0,1);
		
		bulletCollision->SetWorldScale3D(FVector(m_bulletRadius * m_currentCharge));


	}

#pragma endregion
	
	if (!Explosed && m_bIsBulletLaunch)
	{
		TArray<FHitResult> OutHits = MakeSphereCollision(m_bulletRadius * 32);

		if (OutHits.Num())
		{
			DammageOnHits(OutHits, m_bulletDammage);
			Explosion();
			Explosed = true;

			// Niagara Explosion
			if (NS_Explosion)
			{
				// Check if the Niagara FX asset was loaded successfully
				// Set up the spawn parameters
				FVector SpawnLocation = GetActorLocation();
				FVector Scale = FVector(1, 1, 1);
				FRotator SpawnRotation = FRotator::ZeroRotator;
				// Spawn the Niagara FX system at the specified location and rotation
				UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Explosion, SpawnLocation, SpawnRotation,  Scale, true);
			}

			if (SoundSplash)
				UGameplayStatics::SpawnSound2D(GetWorld(), SoundSplash);
		}
	}
	else if(m_bIsBulletLaunch)
	{
		TArray<FHitResult> OutHits = MakeSphereCollision(m_explosionRadius);
		DammageOnHits(OutHits, m_explosionDammage);
	}
}

void ABullet::Explosion()
{
	projectileMovement->StopMovementImmediately();
	if (bulletMesh)
		bulletMesh->DestroyComponent();
	
	SetLifeSpan(m_explosionDuration);
}

void ABullet::SetBulletSetting(float bulletSpeed, int dammage, int explosionDammage, float explosionRadius, float explostionDuration, int lifeSpan, float bulletRadius, float chargeSpeed, AFallenCorsairCharacter* character)
{
	m_bulletDammage = dammage;
	m_explosionDammage = explosionDammage;
	m_explosionRadius = explosionRadius;
	m_bulletSpeed = bulletSpeed;
	m_bulletRadius = bulletRadius;
	m_lifeSpan = lifeSpan;
	m_chargeSpeed = chargeSpeed;
	m_ownerRef = character;
	m_explosionDuration = explostionDuration;
}

void ABullet::LaunchBullet(FVector Dir)
{
	if (Dir == FVector::Zero())
		projectileMovement->Velocity = GetActorForwardVector() * m_bulletSpeed;
	else
		projectileMovement->Velocity = Dir * m_bulletSpeed;

	m_bIsBulletLaunch = true;
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABullet::Explosion, m_lifeSpan, false);

	if (SoundShoot)
		UGameplayStatics::SpawnSound2D(GetWorld(), SoundShoot);
}

TArray<FHitResult> ABullet::MakeSphereCollision(float _SphereRadius)
{
	FVector Start = GetActorLocation();
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(_SphereRadius);
	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(m_ownerRef);

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

void ABullet::DammageOnHits(TArray<FHitResult> OutHits, float DammageValue, FDamageTypeEvent DamageEvent)
{
	for (auto It = OutHits.CreateIterator(); It; It++)
	{
		ACharacter* CharacterHited = Cast<ACharacter>((*It).GetActor());
		if (CharacterHited)
		{
			if (Explosed)
			{
				if (ActorHitedByExplosion.Contains(CharacterHited))
				{
					continue;
				}
				ActorHitedByExplosion.Add(CharacterHited);
			}

			CharacterHited->TakeDamage(DammageValue, DamageEvent, nullptr, GetOwner());
		}
	}
}

bool ABullet::GetIsBulletCharge()
{
	if(this)
		return m_bIsFullyCharge;
	else
		return false;
}

bool ABullet::GetIsBulletLaunch()
{
	return m_bIsBulletLaunch;
}

void ABullet::PlayChargeSound()
{
	if (SoundCharge)
	{
		if (!AudioComponentCharge)
		{
			AudioComponentCharge = UGameplayStatics::SpawnSound2D(GetWorld(), SoundCharge);
		}
	}
}

void ABullet::PlayChargeCompletedSound()
{
	if (!AudioComponentChargeComplete->IsPlaying())
	{
		AudioComponentChargeComplete->Play();
	}
}

void ABullet::StopChargeSound()
{
	StopAudioComponent(AudioComponentCharge);
	StopAudioComponent(AudioComponentChargeComplete);
}

// Private
void ABullet::StopAudioComponent(UAudioComponent* AudioComponent)
{
	if (!AudioComponent)
		return;

	AudioComponent->Stop();
	AudioComponent->DestroyComponent();
}
