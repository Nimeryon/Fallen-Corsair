// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "FallenCorsair/Enemies/AlienBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../FallenCorsairCharacter.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Hit"));

	if (Cast<AFallenCorsairCharacter>(OtherActor))
		return;

	AAlienBase* alien = Cast<AAlienBase>(OtherActor);

	if(alien)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Bullet"));
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
		UE_LOG(LogTemp, Warning, TEXT("Hit Bullet"));
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

	FVector Start = GetActorLocation();
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereRadius);
	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, FQuat::Identity, UEngineTypes::ConvertToTraceType(ECC_Visibility), SphereShape, QueryParams);

	if (Debug)
	{
		FColor Color = FColor::Red;
		if (OutHits.Num())
			Color = FColor::Green;
		DrawDebugSphere(GetWorld(), Start, SphereRadius, 10, Color, false, 1, 0, 1);
	}

	if (OutHits.Num())
	{
		Explosion();
	}
}

void ABullet::Explosion()
{
	projectileMovement->StopMovementImmediately();
	bulletMesh->DestroyComponent();
	// dammageCollision->SetWorldScale3D(FVector(m_dammageRadius));
	SetLifeSpan(ExplosionDuration);
}

void ABullet::SetBulletSetting(float bulletSpeed, int dammage, float dammageRadius, int lifeSpan, float bulletRadius)
{
	m_dammage = dammage;
	m_dammageRadius = dammageRadius;
	projectileMovement->Velocity =  projectileMovement->Velocity * bulletSpeed;

	bulletCollision->SetWorldScale3D(FVector(bulletRadius));
	
	FTimerHandle UnusedHandle;
	// GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABullet::Explosion, lifeSpan, false);
}