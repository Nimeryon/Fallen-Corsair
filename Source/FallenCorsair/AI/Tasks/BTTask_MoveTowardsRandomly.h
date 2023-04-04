// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveTowardsRandomly.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class FALLENCORSAIR_API UBTTask_MoveTowardsRandomly : public UBTTaskNode
{
	GENERATED_BODY()

	/** fixed distance added to threshold between AI and goal location in destination reach test */
	UPROPERTY(config, Category = Node, EditAnywhere, meta=(ClampMin = "0.0", UIMin="0.0"))
	float AcceptableRadius;
	
	/** Max angle range for random direction towards targer */
	UPROPERTY(config, Category = Node, EditAnywhere, meta=(ClampMin = "0.0", UIMin="0.0", ClampMax = "180.0", UIMax="180.0"))
	float AngleRange;

	/** Min segment length to the next point towards target */
	UPROPERTY(config, Category = Node, EditAnywhere, meta=(ClampMin = "10.0", UIMin="10.0"))
	float MinSegmentLength;
	
	/** Max segment length to the next point towars target */
	UPROPERTY(config, Category = Node, EditAnywhere, meta=(ClampMin = "10.0", UIMin="10.0"))
	float MaxSegmentLength;

protected:
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector BlackboardKey;
	
public:
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	virtual FVector GetNextRandomPosition(const AActor* Actor, const FVector& TargetPosition);
	virtual FVector GetTargetPosition(UBehaviorTreeComponent& OwnerComp);
};
