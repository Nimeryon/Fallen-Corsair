// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	bulletMesh->SetupAttachment(RootComponent);

	bulletCollision = CreateDefaultSubobject<USphereComponent>("BulletCollision");
	bulletCollision->SetupAttachment(bulletMesh);
	bulletCollision->SetSphereRadius(40.f);
	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	
	/// bulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnHit);
	/// add an ont hit event
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

}

