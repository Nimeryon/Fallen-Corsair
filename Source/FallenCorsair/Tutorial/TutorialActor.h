// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialActor.generated.h"

class UTutorialStep;

// Event dispatcher OnTutorialComplete
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTutorialComplete);

UCLASS()
class FALLENCORSAIR_API ATutorialActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialActor();

	FTutorialComplete OnTutorialComplete;

	UPROPERTY()
	class AFallenCorsairCharacter* m_player;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Victory Condition")
	int32 m_currentStepIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "Victory Condition")
	FString m_currentStepName = FString();

	UPROPERTY(EditAnywhere, Category = "Victory Condition")
	TArray<TSubclassOf<UTutorialStep>> m_tutorialSteps;

	UPROPERTY(VisibleAnywhere, Category = "Victory Condition")
	bool m_bIsTutorialComplete = false;

public:
	void TutorialComplete();

	void PlayNextStep(bool isFirstStep = false);
};
