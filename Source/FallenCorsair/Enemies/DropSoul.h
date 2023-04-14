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

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	class AFallenCorsairCharacter* m_playerRef;
	
	UPROPERTY(EditAnywhere, Category = "Soul", meta = (ClampMin = 1, UIMin = 1, ClampMax = 2000, UIMax = 2000), meta = (displayName = "Distance d'attraction"))
	float m_distance = 500.f;

	UPROPERTY(EditAnywhere, Category = "Soul", meta = (ClampMin = 1, UIMin = 1, ClampMax = 20, UIMax = 20), meta = (displayName = "Durée de vie"))
	float m_lifeSpan = 5.f;

	UPROPERTY(EditAnywhere, Category = "Soul", meta = (ClampMin = 1, UIMin = 1, ClampMax = 2000, UIMax = 2000), meta = (displayName = "Vitesse d'attraction"))
	float m_speed = 300.f;

public:
	UFUNCTION()
	void ResetLifeSpan();
};
