// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienBase.h"
#include "AlienInhibitor.generated.h"

UCLASS()
class FALLENCORSAIR_API AAlienInhibitor : public AAlienBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAlienInhibitor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	AActor* SplineMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerToDisabled")
	AActor* ASpawner;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void DisabledSpawner();

	virtual void Death(EDamageType DamageType) override;
};
