// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gun.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLENCORSAIR_API UGun : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGun();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/**
	 * This Is The Current Slot Of Ammo
	 */
	UPROPERTY(EditAnywhere, Category = "Gun", meta = (ClampMin = 0, UIMin = 0), meta = (displayName = "Slot de tir actuelle"))
	int m_gunAmmo;

	/**
	 * This Is The Max Slot Of Ammo Available (base 3)
	 */
	UPROPERTY(EditAnywhere,  Category = "Gun", meta = (ClampMin = 1, UIMin = 1), meta = (displayName = "Slot de tir max"))
	int m_maxGunAmmo;

	/**
	 * This Is The Cost Of A Simple Shot (base 1)
	 */
	UPROPERTY(EditAnywhere, Category = "Gun", meta = (ClampMin = 1, UIMin = 1), meta = (displayName = "Coût tir simple"))
	int m_ammoCost;

	/**
	 * This Is The Cost Of A Super Shot (base 3)
	 */
	UPROPERTY(EditAnywhere, Category = "Gun", meta = (ClampMin = 1, UIMin = 1), meta = (displayName = "Coût tir puissant"))
	int m_superAmmoCost;

	UPROPERTY()
	float m_distance;

	UPROPERTY(EditAnywhere, Category = "Gun", meta = (displayName = "Projectile"))
	TSubclassOf<AActor> m_bullet;
	
	UPROPERTY(EditAnywhere, Category = "Barrel", meta = (ClampMin = 1, UIMin = 1), meta = (displayName = "Coût tir puissant"))
    int m_soulGauge;
	
public:
	UFUNCTION()
	int GetAmmo();

	UFUNCTION()
	void SetAmmo(int newAmmo);

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION()
	void SuperShoot();
};
