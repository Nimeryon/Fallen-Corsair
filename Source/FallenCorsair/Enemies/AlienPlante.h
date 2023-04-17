// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienBase.h"
#include "NiagaraSystem.h"
#include "AlienPlante.generated.h"

UCLASS()
// class FALLENCORSAIR_API AAlienPlante : public ACharacter
class FALLENCORSAIR_API AAlienPlante : public AAlienBase
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AAlienPlante();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float Dammage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float PropulsionForce = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float RotationAngleDegrees = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float SphereRadius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	UNiagaraSystem* NS_Explosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	bool Debug = false;

	bool CanExplose = true;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
