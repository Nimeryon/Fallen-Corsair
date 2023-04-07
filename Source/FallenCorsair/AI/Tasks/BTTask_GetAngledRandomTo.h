// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetAngledRandomTo.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class FALLENCORSAIR_API UBTTask_GetAngledRandomTo : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_GetAngledRandomTo(const FObjectInitializer& ObjectInitializer);

	/** Max angle range for random direction towards target */
	UPROPERTY(config, Category = Node, EditAnywhere, meta=(ClampMin = "0.0", UIMin="0.0", ClampMax = "180.0", UIMax="180.0"))
	float AngleRange = 15;

	/** Min segment length to the next point towards target */
	UPROPERTY(config, Category = Node, EditAnywhere, meta=(ClampMin = "10.0", UIMin="10.0"))
	float MinSegmentLength = 1000;
	
	/** Max segment length to the next point towards target */
	UPROPERTY(config, Category = Node, EditAnywhere, meta=(ClampMin = "10.0", UIMin="10.0"))
	float MaxSegmentLength = 2000;

protected:
	/** Target Blackboard Key */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector BBKTarget;

	/** Target Position Blackboard Key */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector BBKTargetPosition;

public:
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual FVector GetNextRandomPosition(const AActor* Actor, const FVector& TargetPosition);
};
