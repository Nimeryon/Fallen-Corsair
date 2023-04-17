// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienPlante.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/Explosion.h"

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
}

// Called to bind functionality to input
void AAlienPlante::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AAlienPlante::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	

	if (CanExplose)
	{
		AAlienBase* Alien = Cast<AAlienBase>(DamageCauser);
		
		if (Alien)
		{
			return 0;
		}

		CanExplose = false;
		UExplosion::PerformExplosion(GetWorld(), GetOwner(), Dammage, GetOwner()->GetActorLocation(), SphereRadius, PropulsionForce, RotationAngleDegrees, NS_Explosion, Debug);
	}

    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

