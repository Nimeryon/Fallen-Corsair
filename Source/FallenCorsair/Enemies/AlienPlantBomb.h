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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float PropulsionForce = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float RotationAngleDegrees = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float CountdownBeforeExplosion = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX Explosion")
	class UNiagaraSystem* NS_Explosion;

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
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bDetectedSomeone = false;
	float CurrentCountdown = 0;

	class USphereComponent* SphereCollisionDetection;

	bool bIsOverlapping;

};
