// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AlienBase.generated.h"

UCLASS()
class FALLENCORSAIR_API AAlienBase : public APawn
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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float m_movementSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float m_attackDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float m_attackSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float m_attackRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float m_attackDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float m_cooldownTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	bool m_bInRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	bool m_bInCooldown;
};
