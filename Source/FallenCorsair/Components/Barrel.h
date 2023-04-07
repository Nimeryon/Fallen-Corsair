// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Barrel.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLENCORSAIR_API UBarrel : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBarrel();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Bombonne", meta = (ClampMin = 0, UIMin = 0), meta = (displayName = "Nombre de slot actuelle"))
	int m_slot = 3;

	UPROPERTY(EditAnywhere, Category = "Bombonne", meta = (ClampMin = 3, UIMin = 3), meta = (displayName = "Nombre max de slot"))
	int m_maxSlot = 3;

	UPROPERTY()
	float m_currentSoul = 0.f;

	UPROPERTY()
	float m_maxSoul = 1.f;

	UPROPERTY()
	float m_ennemyDropSoul = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Bombonne", meta = (ClampMin = 0.01, UIMin = 0.01), meta = (displayName = "Vitesse de chargement en seconde"))
	float m_speedCharge = 0.01f;

public:
	UFUNCTION()
	int GetSlot();

	UFUNCTION()
	void SetSlot(int newSlot);

	UFUNCTION()
	int GetMaxSlot();

	UFUNCTION()
	void CollectSoul();
		
};