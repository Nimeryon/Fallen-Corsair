// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialActor.generated.h"

UENUM()
enum class EActionType
{
	LightAttack,
	HeavyAttack,
	SoulPickup,
	Shoot,
	Dodge,
};

USTRUCT()
struct FTutorialStep
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ConditionType == EConditionType::ActionPerformed", EditConditionHides))
	EActionType ActionPerformed;

	UPROPERTY(VisibleAnywhere)
	int32 Count;

	UPROPERTY(EditAnywhere)
	int32 Goal;

	UPROPERTY(EditAnywhere)
	bool bIsStepComplete;
	
};

UCLASS()
class FALLENCORSAIR_API ATutorialActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class AFallenCorsairCharacter* m_player;

	UPROPERTY()
	FTutorialStep m_currentStep;

public:	
	UFUNCTION()
	OnLightAttack();

	UFUNCTION()
	OnHeavyAttack();

	UFUNCTION()
	OnSoulPickup();
	
	UFUNCTION()
	OnShoot();

	UFUNCTION()
	OnDodge();
};
