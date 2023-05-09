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

#pragma region Variables
	/**
	 * This Is The Current Slot Of Ammo
	 */
	UPROPERTY()
	int m_gunAmmo;

	/**
	 * This Is The Max Slot Of Ammo Available (base 3)
	 */
	UPROPERTY()
	int m_maxGunAmmo;

	/**
	 * This Is The Cost Of A Simple Shot (base 1)
	 */
	UPROPERTY(EditAnywhere, Category = "Gun", meta = (ClampMin = 1, UIMin = 1), meta = (displayName = "Coût tir simple"))
	int m_ammoCost = 1;

	UPROPERTY()
	float m_distance;

	UPROPERTY()
	class ABullet* m_spawnBullet;
	
	/**
	 * Place the BP_Bullet, Nothing else
	 */
	UPROPERTY(EditAnywhere, Category = "Gun|Bullet", meta = (displayName = "Projectile"))
	TSubclassOf<class ABullet> m_bullet;

	UPROPERTY()
	class AFallenCorsairCharacter* m_ownerRef;

	UPROPERTY()
	class UBarrel* m_barrelRef;

	UPROPERTY(EditAnywhere, Category = "Gun|Bullet", meta = (displayName = "Vitesse du projectile"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 20000, UIMax = 20000))
	float m_bulletSpeed = 2000;

	UPROPERTY(EditAnywhere, Category = "Gun|Bullet", meta = (displayName = "Dégât du projectile"), meta = (ClampMin = 1, UIMin = 1))
	int m_bulletDammage = 1;

	UPROPERTY(EditAnywhere, Category = "Gun|Bullet", meta = (displayName = "Dégât de l'explosion du projectile"), meta = (ClampMin = 1, UIMin = 1))
	int m_explosionDammage = 1;

	UPROPERTY(EditAnywhere, Category = "Gun|Bullet", meta = (displayName = "Portée de l'explosion du projectile"))
	float m_bulletExplosionRadius = 5.f;

	UPROPERTY(EditAnywhere, Category = "Gun|Bullet", meta = (displayName = "Durée de vie du projectile"), meta = (ClampMin = 1, UIMin = 1, ClampMax = 10, UIMax = 10))
	int m_bulletLifeSpan = 2;

	UPROPERTY(EditAnywhere, Category = "Gun|Bullet", meta = (displayName = "Taille du projectile"), meta = (ClampMin = 0.5, UIMin = 0.5, ClampMax = 3, UIMax = 3))
	float m_bulletRadius = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Gun|Bullet", meta = (displayName = "Durée de l'explosion"), meta = (ClampMin = 1, UIMin = 1))
	float m_explosionDuration = 1;
	
	UPROPERTY(EditAnywhere, Category = "Gun|Bullet", meta = (displayName = "Vitesse de chargement"), meta = (ClampMin = 0.1, UIMin = 0.1, ClampMax = 10, UIMax = 10))
	float m_ChargeSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category = "Gun|Bullet", meta = (displayName = "Socket spawn location"))
	FName m_socketLoc;

#pragma endregion 
	
public:
	UFUNCTION()
	int GetAmmo();

	UFUNCTION()
	void SetAmmo(int newAmmo);

	UFUNCTION()
	void Shoot();

	UFUNCTION()
	void StopCharge(bool bIsCancel);

	UFUNCTION()
	FVector GetDirectionBullet();

	
private:
	FVector GetImpactPointOnScreen();
	class UCameraComponent* GetParentCamera();

	bool shooted = false;
	
};