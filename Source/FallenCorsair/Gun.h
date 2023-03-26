// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gun.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	UPROPERTY(EditAnywhere, Category = "Gun", meta = (ClampMin = 0, UIMin = 0))
	int m_gunCharge;
	
	UPROPERTY(EditAnywhere,  Category = "Gun", meta = (ClampMin = 1, UIMin = 1))
	int m_maxGunCharge;

	UPROPERTY(EditDefaultsOnly)
	float m_distance;

public:
	UFUNCTION()
	int GetCharge();

	UFUNCTION()
	void SetCharge(int newCharge);

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void SuperFire();
};
