// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutorialStep.generated.h"

UENUM()
enum class ETutorialActionType
{
	LightAttack,
	HeavyAttack,
	SoulPickup,
	Shoot,
	Dodge,
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class FALLENCORSAIR_API UTutorialStep : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void Initialize(class ATutorialActor* tutorial, class AFallenCorsairCharacter* player);

	UPROPERTY(EditAnywhere, Category = "Tutorial Step")
	class ATutorialActor* m_tutorial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial Step")
	class AFallenCorsairCharacter* m_player;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial Step")
	FString m_name = "step";
	
	UPROPERTY(EditAnywhere, Category = "Tutorial Step")
	ETutorialActionType m_actionPerformed = ETutorialActionType::LightAttack;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial Step")
	int32 m_count = 0;

	UPROPERTY(EditAnywhere, Category = "Tutorial Step")
	int32 m_goal = 1;

	UPROPERTY()
	bool m_bIsActive = false;
	
	UPROPERTY(EditAnywhere, Category = "Tutorial Step")
	bool m_bIsStepComplete = false;

	UFUNCTION()
	void CheckStepComplete();

	UFUNCTION()
	void OnStepComplete();

	UFUNCTION(BlueprintNativeEvent, Category = "Tutorial Step")
	void OnStepCompleteBP();

	UFUNCTION()
	void OnStepBegin();

	UFUNCTION(BlueprintNativeEvent, Category = "Tutorial Step")
	void OnStepBeginBP();
};
