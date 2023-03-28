// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToCircular.generated.h"

/**
 * 
 */
UCLASS()
class FALLENCORSAIR_API UBTTask_MoveToCircular : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

protected:
	virtual EBTNodeResult::Type PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual UAITask_MoveTo* PrepareMoveTask(UBehaviorTreeComponent& OwnerComp, UAITask_MoveTo* ExistingTask, FAIMoveRequest& MoveRequest) override;
};
