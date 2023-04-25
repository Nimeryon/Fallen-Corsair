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
	virtual void Destroyed() override;

	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnNotifyReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	
	UFUNCTION(BlueprintCallable)
	virtual bool Attack();

	UFUNCTION(BlueprintCallable)
	virtual bool CreateAvoidBox();
	
	UFUNCTION()
	virtual AlienState GetState() const;
	
	UFUNCTION()
	virtual void SetState(AlienState State);
	
public:
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FVector m_attackBoxSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FVector m_attackBoxOffset;
#pragma endregion

#pragma region Hit Chance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Chance", meta = (ClampMin = "0", UIMin = "0"))
	FVector m_avoidBoxSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Chance", meta = (ClampMin = "0", UIMin = "0"))
	FVector m_avoidBoxOffset;
#pragma endregion

#pragma region Jump Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackJumpHeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0", UIMin = "0"))
	float m_attackJumpSpeed;
#pragma endregion

private:
	UAnimInstance* GetAnimInstance() const;
};
