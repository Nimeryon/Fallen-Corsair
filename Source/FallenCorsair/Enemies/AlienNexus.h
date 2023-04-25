// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienBase.h"
#include "AlienNexus.generated.h"

UCLASS()
class FALLENCORSAIR_API AAlienNexus : public AAlienBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAlienNexus();



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AlienToDestroyBefore")
	TArray<AAlienBase*> AliensToDestroyBefore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoRegeneration")
	float AutoRegenerationCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoRegeneration")
	float GainHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Mesh")
	UStaticMeshComponent* MeshShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dammage Reduction while Shield")
	float DammageReductionWhileShield = 0.25;

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

	virtual void AutoRegeneration();
	virtual bool bAllAliensAreDestroyed() const;

	float CurrentTimeAutoRegeneration;
};
