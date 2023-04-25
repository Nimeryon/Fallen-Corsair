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
	
	UFUNCTION()    
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Sphere Collision")
	bool Debug = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere Radius Collision")
	float BulletSphereRadius = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere Radius Collision")
	float ExplosionSphereRadius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dammage")
	float BulletDammage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dammage")
	float ExplosionDammage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion Duration")
	float ExplosionDuration = 1;

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
	float m_dammageRadius = 5.f;

	UPROPERTY()
	int m_dammage = 1;

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
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void Explosion();

	UFUNCTION()
	void SetBulletSetting(float bulletSpeed, int dammage, float dammageRadius, int lifeSpan, float bulletRadius, float chargeSpeed, AFallenCorsairCharacter* character);

	UFUNCTION()
	void LaunchBullet();

	UFUNCTION()
	bool GetIsBulletCharge();
	
	void DammageOnHits(TArray<FHitResult> OutHits, float DammageValue, FDamageTypeEvent DamageEvent = EDamageType::Default);

	TArray<FHitResult> MakeSphereCollision(float _SphereRadius);

	AActor *OwnerCauser;

	bool Explosed = false;
	TArray<class AActor*> ActorHitedByExplosion;

};