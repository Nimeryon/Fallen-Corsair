// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlienBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlienSpawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlienDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlienDeathWithActor, AAlienBase*, Alien);

UCLASS()
class FALLENCORSAIR_API AAlienBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAlienBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY()
	FOnAlienSpawn OnSpawn;
	
	UPROPERTY()
	FOnAlienDeath OnDeath;

	UPROPERTY()
	FOnAlienDeathWithActor OnDeathWithActor;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "1", UIMin = "1"))
	float m_health = 15;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float m_currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	bool m_destroyOnDeath = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float m_movementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float m_attackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float m_attackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float m_attackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float m_attackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float m_cooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool m_bInRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool m_bInCooldown;
};
