// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsAlienStunned.generated.h"

/**
 * 
 */
UCLASS()
class FALLENCORSAIR_API UBTDecorator_IsAlienStunned : public UBTDecorator
{
	GENERATED_BODY()
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
