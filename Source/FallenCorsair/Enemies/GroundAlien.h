// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienBase.h"
#include "GroundAlien.generated.h"

/**
 * 
 */
UCLASS()
class FALLENCORSAIR_API AGroundAlien : public AAlienBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float m_combatModeRange;
};
