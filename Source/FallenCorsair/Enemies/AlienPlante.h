// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienBase.h"
#include "AlienPlante.generated.h"

UCLASS()
// class FALLENCORSAIR_API AAlienPlante : public ACharacter
class FALLENCORSAIR_API AAlienPlante : public AAlienBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAlienPlante();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* SM_PlanteAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* SM_PlanteUnlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Revive")
	float ReviveCooldown = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dammage")
	float Dammage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Collider Sphere")
	float SphereRadius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Collider Sphere")
	bool Debug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_reviveSound;

protected:
	bool CanEffect = true; // Permit the plant to explose or gaz

	virtual TArray<FHitResult> MakeSphereCollision(float _SphereRadius);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	float ReviveTimer = 0;
	bool bCanPlaySoundDestroy = true;
};
