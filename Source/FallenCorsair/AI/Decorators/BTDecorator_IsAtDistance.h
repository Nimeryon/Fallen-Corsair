// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsAtDistance.generated.h"

/**
 * 
 */
UCLASS()
class FALLENCORSAIR_API UBTDecorator_IsAtDistance : public UBTDecorator
{
	GENERATED_BODY()
	
	UBTDecorator_IsAtDistance(const FObjectInitializer& ObjectInitializer);

	/** Distance to check */
	UPROPERTY(EditAnywhere, Category = Condition)
	float Distance = 2000;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	/** Target Blackboard Key */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BBKTarget;
};
