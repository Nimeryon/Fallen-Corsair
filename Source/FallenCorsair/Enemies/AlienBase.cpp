// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienBase.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAlienBase::AAlienBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAlienBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = m_movementSpeed;
	m_currentHealth = m_health;
}

// Called every frame
void AAlienBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAlienBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AAlienBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	m_currentHealth -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("%d"), m_currentHealth);
	if (m_currentHealth <= 0)
		Destroy();
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}