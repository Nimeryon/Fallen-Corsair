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
	AGroundAlien(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	virtual bool Attack(AActor* Target);
	
	UFUNCTION()
	virtual AlienState GetState() const;
	
	UFUNCTION()
	virtual void SetState(AlienState State);
	
public:
#pragma region Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UMelee* m_meleeComponent;
#pragma endregion

#pragma region State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	AlienState m_state = AlienState::Normal;
#pragma endregion

#pragma region Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_missCooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_hitCooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool m_bIsInCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float m_cooldownTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float m_currentCooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	AActor* m_attackTarget = nullptr;
#pragma endregion

#pragma region Jump Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackJumpHeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackJumpSpeed;
#pragma endregion
};
