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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* SoundBarrierDisparition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* SoundBarrierHitPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* SoundBarrierNoise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* SoundNoyauDeath;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

	virtual void AutoRegeneration();
	virtual bool bAllAliensAreDestroyed() const;

	float CurrentTimeAutoRegeneration;

	bool bCanPlaySoundDeath = true;
	bool bCanPlayBarrierDisparition = true;
};
