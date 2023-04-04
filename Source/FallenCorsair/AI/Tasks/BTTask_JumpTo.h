// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_JumpTo.generated.h"

/**
 * 
 */
UCLASS()
class FALLENCORSAIR_API UBTTask_JumpTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

protected:
	virtual EBTNodeResult::Type PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};