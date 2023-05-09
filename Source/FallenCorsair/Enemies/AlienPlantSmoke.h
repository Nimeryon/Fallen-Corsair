// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienPlante.h"
#include "GameFramework/Character.h"
#include "AlienPlantSmoke.generated.h"

UCLASS()
class FALLENCORSAIR_API AAlienPlantSmoke : public AAlienPlante
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAlienPlantSmoke();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX Smoke")
	float StunDuration = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX Smoke")
	class UNiagaraSystem* NS_Smoke;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* SoundSmoke;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void StunAlien();
};
