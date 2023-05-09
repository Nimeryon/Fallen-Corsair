// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienBase.h"
#include "GroundAlien.generated.h"

UENUM()
enum class AlienState
{
	Normal,
	Attack
};

UCLASS()
class FALLENCORSAIR_API AGroundAlien : public AAlienBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	virtual void PrepareAttack();
	
	UFUNCTION(BlueprintCallable)
	virtual bool Attack();

	UFUNCTION(BlueprintCallable)
	virtual bool CreateAvoidBox();

	UFUNCTION(BlueprintCallable)
	virtual void JumpTowardsTarget();
	
	UFUNCTION(BlueprintCallable)
	virtual void SetCooldownTime(float Cooldown);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetCooldownActive();

	UFUNCTION(BlueprintCallable)
	virtual bool IsInCooldown() const;
	
	UFUNCTION()
	virtual AlienState GetState() const;
	
	UFUNCTION(BlueprintCallable)
	virtual void ForceDeath();
	
	UFUNCTION()
	virtual void SetState(AlienState State);

	virtual void Death(EDamageType DamageType) override;

	virtual bool Stun(float Time) override;

	virtual void Unstunned() override;

	virtual void PlayDeathFX() override;

	UFUNCTION()
	virtual void PlayStunMontage();

	UFUNCTION(BlueprintCallable)
	virtual void AttachAttackFX();
	
	UFUNCTION(BlueprintCallable)
	virtual void ResetCollisionPresset();
	
public:
#pragma region Animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UAnimMontage* m_stunMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UAnimMontage* m_deathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (ClampMin = "0", UIMin = "0"))
	float m_blendTime = 0.1f;
#pragma endregion

#pragma region Rotations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotations", meta = (ClampMin = "0", UIMin = "0"))
	float m_rotationSpeed;
#pragma endregion

#pragma region Soul
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
	TSubclassOf<class ADropSoul> m_soul;
#pragma endregion

#pragma region Material
	UPROPERTY()
	class UMaterialInstanceDynamic* m_material;
#pragma endregion

#pragma region State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	AlienState m_state = AlienState::Normal;
#pragma endregion

#pragma region Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<FName> m_attackSocketsName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_missCooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_hitCooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool m_bIsInCooldown;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float m_cooldownTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float m_currentCooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	AActor* m_attackTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FVector m_attackBoxSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FVector m_attackBoxOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	FVector m_attackTargetPosition;
#pragma endregion

#pragma region Hit Chance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Chance", meta = (ClampMin = "0", UIMin = "0"))
	FVector m_avoidBoxSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Chance", meta = (ClampMin = "0", UIMin = "0"))
	FVector m_avoidBoxOffset;
#pragma endregion

#pragma region Jump Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackMinJumpForce = 300;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackMaxJumpForce = 500;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackMinJumpSpeed = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackMaxJumpSpeed = 1400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackMinJumpDistance = 400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackMaxJumpDistance = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Attack", meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
	float m_attackAngleVariationChance = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackRandomAngleVariation = 15;
#pragma endregion

#pragma region Scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale", meta = (ClampMin = "-1", UIMin = "-1", ClampMax = "1", UIMax = "1"))
	float m_minScaleVariation = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale", meta = (ClampMin = "-1", UIMin = "-1", ClampMax = "1", UIMax = "1"))
	float m_maxScaleVariation = 0.15f;

	UPROPERTY()
	float m_scaleVariation;
#pragma endregion

#pragma region Color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	bool m_bmulticolor = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (ClampMin = "0", UIMin = "0"))
	float m_hueShiftSpeed = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (ClampMin = "0", UIMin = "0", ClampMax = "100", UIMax = "100"))
	float m_hueColor = 0;
#pragma endregion
	
#pragma region FX
	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* m_attackParticle;
#pragma endregion 
};
