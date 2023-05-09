// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "AlienBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlienSpawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlienDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlienDeathWithActor, AAlienBase*, Alien);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlienStunned, float, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlienDamaged, float, Damage);

#pragma region DamageType
UENUM()
enum class EDamageType
{
	MeleeSoft,
	MeleeHeavy,
	Distance,
	Explosion,
	Default
};

USTRUCT()
struct FDamageTypeEvent : public FDamageEvent
{
	GENERATED_USTRUCT_BODY();
	
	FDamageTypeEvent(EDamageType Type = EDamageType::Default) : DamageType(Type) {};

	UPROPERTY(EditAnywhere)
	EDamageType DamageType;
};
#pragma endregion 

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

	UFUNCTION()
	virtual bool IsAlive() const;

	UFUNCTION()
	virtual bool IsStunned() const;

	UFUNCTION()
	virtual float GetRemainingStunTime() const;

	UFUNCTION()
	virtual float GetStunTime() const;

	UFUNCTION()
	virtual bool Stun(float Time);

	UFUNCTION()
	virtual void Unstunned();

	UFUNCTION(BlueprintCallable)
	virtual void Kill();

	UFUNCTION()
	virtual float GetDamageMultiplicator(EDamageType DamageType) const;

	UFUNCTION(BlueprintCallable)
	virtual void Death(EDamageType DamageType = EDamageType::Default);

	UFUNCTION(BlueprintCallable)
	virtual void PlayDeathFX();

	virtual void FellOutOfWorld(const UDamageType& dmgType) override;
	
	UPROPERTY()
	FOnAlienSpawn OnSpawn;
	
	UPROPERTY()
	FOnAlienDeath OnDeath;

	UPROPERTY()
	FOnAlienDeathWithActor OnDeathWithActor;
	
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

#pragma region Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool m_bCanTakeDamage = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0", UIMin = "0", EditCondition="m_bCanTakeDamage"))
	float m_attackMeleeSoftMultiplicator = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0", UIMin = "0", EditCondition="m_bCanTakeDamage"))
	float m_attackMeleeHeavyMultiplicator = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0", UIMin = "0", EditCondition="m_bCanTakeDamage"))
	float m_attackDistanceMultiplicator = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0", UIMin = "0", EditCondition="m_bCanTakeDamage"))
	float m_attackExplosionMultiplicator = 1;
#pragma endregion
	
#pragma region Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float m_minMovementSpeed = 800;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float m_maxMovementSpeed = 1000;

	UPROPERTY(BlueprintReadOnly)
	float m_movementSpeed;
#pragma endregion

#pragma region FX

	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* m_hurtParticle;

	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* m_deathParticle;

#pragma endregion 
	
};
