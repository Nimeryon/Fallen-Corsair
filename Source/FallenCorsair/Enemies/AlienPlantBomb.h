// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienPlante.h"
#include "GameFramework/Character.h"
#include "AlienPlantBomb.generated.h"

UCLASS()
class FALLENCORSAIR_API AAlienPlantBomb : public AAlienPlante
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAlienPlantBomb();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere radius detection")
		float DetectionSphereRadius = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		float PropulsionForce = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		float RotationAngleDegrees = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
		float CountdownBeforeExplosion = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX Explosion")
		class UNiagaraSystem* NS_Explosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundWave* SoundDetonator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundExplosion;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	bool bDetectedSomeone = false;
	float CurrentCountdown = 0;

	bool bIsOverlapping;

	virtual void DetectPlayer();

	UAudioComponent* AudioComponentDetonator;
};
