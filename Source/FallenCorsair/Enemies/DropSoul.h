// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropSoul.generated.h"

UCLASS()
class FALLENCORSAIR_API ADropSoul : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	class UStaticMeshComponent* soulMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	class USphereComponent* soulCollision;
	
public:	
	// Sets default values for this actor's properties
	ADropSoul();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Soul, meta = (ClampMin = "0", UIMin = "0"))
	float m_pickupTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Soul, meta = (ClampMin = "0", UIMin = "0"))
	float m_destroyTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Soul, meta = (ClampMin = "0", UIMin = "0"))
	float m_pickupDistance;
	
private:

	UPROPERTY()
	class AFallenCorsairCharacter* m_playerRef;
	
	UPROPERTY(EditAnywhere, Category = "Soul", meta = (ClampMin = 1, UIMin = 1, ClampMax = 2000, UIMax = 2000), meta = (displayName = "Distance d'attraction"))
	float m_distance = 500.f;

	/**
	 * If Set To 0, It will never destroy
	 */
	UPROPERTY(EditAnywhere, Category = "Soul", meta = (ClampMin = 1, UIMin = 1, ClampMax = 20, UIMax = 20), meta = (displayName = "Durée de vie"))
	float m_lifeSpan = 5.f;

	UPROPERTY(EditAnywhere, Category = "Soul", meta = (ClampMin = 1, UIMin = 1, ClampMax = 2000, UIMax = 2000), meta = (displayName = "Vitesse d'attraction"))
	float m_speed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = Soul)
	bool m_bCanBePickedUp = false;
	
	UPROPERTY(EditAnywhere, Category = Soul)
	bool m_bPickedUp = false;

	UPROPERTY(EditDefaultsOnly, Category = Soul)
	float m_currentPickupTime = 0;

	UPROPERTY(EditDefaultsOnly, Category = Soul)
	float m_currentDestroyTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* SoundAbsorbtionSoul;

public:
	UFUNCTION()
	void ResetLifeSpan();
};
