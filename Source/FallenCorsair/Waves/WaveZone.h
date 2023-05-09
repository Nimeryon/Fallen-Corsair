// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveZone.generated.h"

USTRUCT()
struct FEnemies
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<class AAlienBase> Enemy;
	
	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Ratio = 1.f;
};

UCLASS()
class FALLENCORSAIR_API AWaveZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	bool IsPlayerInZone() const;

	UFUNCTION()
	void OnPlayerSpawn();

	UFUNCTION()
	TSubclassOf<class AAlienBase> GetAlienToSpawn() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_bIsActive = true;

protected:
	UPROPERTY()
	AActor* m_player;
	
	UPROPERTY()
	bool m_bIsPlayerInZone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UWaveTracker* m_waveTracker;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* m_collisionBox;

	UPROPERTY(EditAnywhere)
	TArray<FEnemies> m_enemies;
public:
	
};
