// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienPlante.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAlienPlante::AAlienPlante()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAlienPlante::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAlienPlante::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsAlive())
	{
		ReviveTimer += DeltaTime;
	}

	if (ReviveTimer >= ReviveCooldown)
	{
		CanEffect = true;
		bCanPlaySoundDestroy = true;
		m_currentHealth = m_health;
		ReviveTimer = 0;
		if (SoundRevive)
			UGameplayStatics::SpawnSound2D(GetWorld(), SoundRevive);
	}
}

// Called to bind functionality to input
void AAlienPlante::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AAlienPlante::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	
	float ActualDammage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
	if (!bIsAlive() && bCanPlaySoundDestroy)
	{
		bCanPlaySoundDestroy = false;
		if (SoundDestroy)
			UGameplayStatics::SpawnSound2D(GetWorld(), SoundDestroy);
	}

	return ActualDammage;
}

TArray<FHitResult> AAlienPlante::MakeSphereCollision(float _SphereRadius)
{
	FVector Start = GetActorLocation();
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(_SphereRadius);
	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

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