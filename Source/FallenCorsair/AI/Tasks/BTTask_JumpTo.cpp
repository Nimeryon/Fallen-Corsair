// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_JumpTo.h"

void UBTTask_JumpTo::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

EBTNodeResult::Type UBTTask_JumpTo::PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::PerformMoveTask(OwnerComp, NodeMemory);
}
