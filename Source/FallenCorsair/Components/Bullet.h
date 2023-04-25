// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	
};