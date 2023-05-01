// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enemies/AlienBase.h"
#include "Bullet.generated.h"

UCLASS()
class FALLENCORSAIR_API ABullet : public AActor
{
	GENERATED_BODY()

	
	
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	class UStaticMeshComponent* bulletMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	class USphereComponent* bulletCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Dammage Collision")
	class USphereComponent* dammageCollision;
	
	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* projectileMovement;

	

	
	
public:	
	// Sets default values for this actor's properties
	ABullet();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Sphere Collision")
	bool Debug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion FX")
	class UNiagaraSystem* NS_Explosion;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	class AFallenCorsairCharacter* m_ownerRef;

	UPROPERTY()
	bool m_bIsFullyCharge = false;
	
	UPROPERTY()
	float m_explosionRadius = 5.f;

	UPROPERTY()
	int m_bulletDammage = 1;

	UPROPERTY()
	int m_explosionDammage = 1;

	UPROPERTY()
	int m_explosionDuration = 1;

	UPROPERTY()
	float m_lifeSpan;

	UPROPERTY()
	float m_bulletRadius;

	UPROPERTY()
	float m_bulletSpeed;

	UPROPERTY()
	float m_chargeSpeed = 1.f;

	UPROPERTY()
	bool m_bIsCharging = true;

	UPROPERTY()
	float m_currentCharge = 0;

	UPROPERTY()
	bool m_bIsBulletLaunch = false;

	UPROPERTY()
	bool m_isLifeSpanDone = false;

	UPROPERTY(EditAnywhere, Category = "impulse")
	float m_impulse = 1000;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void Explosion();

	UFUNCTION()
	void SetBulletSetting(float bulletSpeed, int dammage, int explosionDammage, float explosionRadius, float explostionDuration, int lifeSpan, float bulletRadius, float chargeSpeed, AFallenCorsairCharacter* character);

	UFUNCTION()
	void LaunchBullet();

	UFUNCTION()
	bool GetIsBulletCharge();

	UFUNCTION()
	bool GetIsBulletLaunch();

	UFUNCTION()
	void IsLifeSpanDone();
	
	void DammageOnHits(TArray<FHitResult> OutHits, float DammageValue, FDamageTypeEvent DamageEvent = EDamageType::Default);

	TArray<FHitResult> MakeSphereCollision(float _SphereRadius);

	bool Explosed = false;
	TArray<class AActor*> ActorHitedByExplosion;

};