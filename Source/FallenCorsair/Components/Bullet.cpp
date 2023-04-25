// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "FallenCorsair/Enemies/AlienBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../FallenCorsairCharacter.h"
#include "Camera/CameraComponent.h"

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
	
	bulletCollision->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	dammageCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::ABullet::OnOverlapBegin);
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit"));
	Explosion();
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAlienBase* alien = Cast<AAlienBase>(OtherActor);
	
	if(alien)
	{
		FDamageEvent DamageEvent;
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

	//m_ownerRef = Cast<AFallenCorsairCharacter>(GetOwner());
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bIsBulletLaunch)
	{
		SetActorLocation(m_ownerRef->GetActorLocation() + FVector(0,0, 100) + m_ownerRef->GetActorForwardVector() * 100);
		SetActorRotation(m_ownerRef->GetFollowCamera()->GetComponentRotation());
	}
	
	if(m_bIsCharging && m_ownerRef)
	{
		if(currentCharge >= 1)
		{
			m_bIsFullyCharge = true;
			m_bIsCharging = false;
		}
		
		FMath::Clamp(currentCharge = currentCharge + 1 / m_chargeSpeed * DeltaTime,0,1);
		
		bulletCollision->SetWorldScale3D(FVector(m_bulletRadius * currentCharge));
	}
}

void ABullet::Explosion()
{
	projectileMovement->StopMovementImmediately();
	bulletMesh->DestroyComponent();
	dammageCollision->SetWorldScale3D(FVector(m_dammageRadius));
	SetLifeSpan(0.2f);
}

void ABullet::SetBulletSetting(float bulletSpeed, int dammage, float dammageRadius, int lifeSpan, float bulletRadius, float chargeSpeed, AFallenCorsairCharacter* character)
{
	m_dammage = dammage;
	m_dammageRadius = dammageRadius;
	
	m_bulletSpeed = bulletSpeed;
	
	m_bulletRadius = bulletRadius;
	m_lifeSpan = lifeSpan;
	m_chargeSpeed = chargeSpeed;
	m_ownerRef = character;
}

void ABullet::LaunchBullet()
{
	projectileMovement->Velocity =  GetActorForwardVector() * m_bulletSpeed;
	bIsBulletLaunch = true;
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABullet::Explosion, m_lifeSpan, false);
}

bool ABullet::GetIsBulletCharge()
{
	return m_bIsFullyCharge;
}
