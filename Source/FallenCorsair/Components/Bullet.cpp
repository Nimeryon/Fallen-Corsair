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
	
	// bulletCollision->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	// dammageCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::ABullet::OnOverlapBegin);
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<AFallenCorsairCharacter>(OtherActor))
		return;

	AAlienBase* alien = Cast<AAlienBase>(OtherActor);

	if (alien)
	{
		const FDamageTypeEvent DamageEvent(EDamageType::Distance);
		OtherActor->TakeDamage(m_dammage, DamageEvent, nullptr, this);
	}
	
	Explosion();
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAlienBase* alien = Cast<AAlienBase>(OtherActor);

	if(alien)
	{
		const FDamageTypeEvent DamageEvent(EDamageType::Distance);
		OtherActor->TakeDamage(m_dammage, DamageEvent, nullptr, this);
	}
	
	/// NON
	/// Add damage to the actor overlap (Ennemy)
	///if(OtherActor->ActorHasTag("ennemy"))
	///{
		/// impulse alien ? or call directly function in alien class ?
		//OtherActor->Destroy();
		///FDamageEvent DamageEvent;
		///OtherActor->TakeDamage(1, DamageEvent, nullptr, this);
	///}
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Explosed)
	{
		TArray<FHitResult> OutHits = MakeSphereCollision(BulletSphereRadius);

		if (OutHits.Num())
		{
			DammageOnHits(OutHits, BulletDammage);
			projectileMovement->StopMovementImmediately();
			bulletMesh->DestroyComponent();
			SetLifeSpan(ExplosionDuration);
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
		}
	}
	else {
		TArray<FHitResult> OutHits = MakeSphereCollision(ExplosionSphereRadius);
		DammageOnHits(OutHits, ExplosionDammage);
	}
}

void ABullet::Explosion()
{
	projectileMovement->StopMovementImmediately();
	bulletMesh->DestroyComponent();
	// dammageCollision->SetWorldScale3D(FVector(m_dammageRadius));
	SetLifeSpan(ExplosionDuration);
}

void ABullet::SetBulletSetting(AActor *_OwnerCauser, float bulletSpeed, int dammage, float dammageRadius, int lifeSpan, float bulletRadius)
{
	OwnerCauser = _OwnerCauser;
	m_dammage = dammage;
	m_dammageRadius = dammageRadius;
	projectileMovement->Velocity =  projectileMovement->Velocity * bulletSpeed;

	bulletCollision->SetWorldScale3D(FVector(bulletRadius));
	
	FTimerHandle UnusedHandle;
	// GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABullet::Explosion, lifeSpan, false);
}

TArray<FHitResult> ABullet::MakeSphereCollision(float _SphereRadius)
{
	FVector Start = GetActorLocation();
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(_SphereRadius);
	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwnerCauser);

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