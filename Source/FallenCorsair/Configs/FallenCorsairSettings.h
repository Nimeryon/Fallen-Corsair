// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FallenCorsairSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class FALLENCORSAIR_API UFallenCorsairSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config)
	float MaxAICount;

	UPROPERTY(EditAnywhere, Config)
	TSoftObjectPtr<UWorld> World;
};

UCLASS()
class UConfigUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float GetMaxAICount();
};
