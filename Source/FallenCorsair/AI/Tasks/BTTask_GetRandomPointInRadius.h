// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetRandomPointInRadius.generated.h"

/**
 * 
 */
UCLASS()
class FALLENCORSAIR_API UBTTask_GetRandomPointInRadius : public UBTTaskNode
{
	GENERATED_BODY()
	UBTTask_GetRandomPointInRadius(const FObjectInitializer& ObjectInitializer);
	
	/** Min distance for generating random point */
	UPROPERTY(Category = Node, EditAnywhere, meta=(ClampMin = "0", UIMin="0"))
	float MinDistance = 1000;
	
	/** max distance for generating random point */
	UPROPERTY(Category = Node, EditAnywhere, meta=(ClampMin = "0", UIMin="0"))
	float MaxDistance = 2000;

	UPROPERTY(Category = Node, EditAnywhere)
	bool GenerateAroundTarget = false;
	
	/** Draw debug point */
	UPROPERTY(Category = Node, EditAnywhere)
	bool DrawDebugPoint = true;

protected:
	/** Target Blackboard Key */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BBKTarget;

	/** Target Position Blackboard Key */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BBKTargetPosition;
	
public:
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
