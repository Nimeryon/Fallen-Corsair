// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FallenCorsair/DT_FallenCorsair.h"
#include "GameFramework/Character.h"
#include "AlienBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlienSpawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlienDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlienStunned, float, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlienDamaged, float, Damage);

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

	virtual bool IsAlive() const;

	virtual bool IsStunned() const;

	virtual float GetRemainingStunTime() const;

	virtual float GetStunTime() const;

	virtual bool Stun(float Time);

	virtual float GetDamageMultiplicator(EDamageType DamageType) const;
	
	UPROPERTY()
	FOnAlienSpawn OnSpawn;
	
	UPROPERTY()
	FOnAlienDeath OnDeath;

	bool bIsAlive() const;
	
	UPROPERTY()
	FOnAlienStunned OnStunned;
	
	UPROPERTY()
	FOnAlienDamaged OnDamaged;
	
public:
#pragma region Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "1", UIMin = "1"))
	float m_health = 15;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float m_currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	bool m_destroyOnDeath;
#pragma endregion

#pragma region Stun
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
	bool m_bCanBeStunned;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun", meta = (EditCondition="m_bCanBeStunned"))
	bool m_bStunned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun", meta = (EditCondition="m_bCanBeStunned"))
	float m_stunTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stun", meta = (EditCondition="m_bCanBeStunned"))
	float m_currentStunTime;
#pragma endregion

#pragma region Multiplicator
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiplicator", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackMeleeSoftMultiplicator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiplicator", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackMeleeHeavyMultiplicator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiplicator", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackDistanceMultiplicator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiplicator", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackExplosionMultiplicator;
#pragma endregion
	
#pragma region Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float m_movementSpeed;
#pragma endregion

#pragma region Attack
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
#pragma endregion
};
