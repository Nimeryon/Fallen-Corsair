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
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	/// create a mesh, a sphere collision, a projectile movement
	/// create an on hit event
	/// expand the collision (collision radius) and spawn a vfx then destroy the bullet
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* projectileMovement;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Bullet")
	float bulletSpeed;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	float damageRadius;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	int damage;
};
