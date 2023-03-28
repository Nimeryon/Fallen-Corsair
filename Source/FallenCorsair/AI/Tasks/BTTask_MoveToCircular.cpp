// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToCircular.h"

void UBTTask_MoveToCircular::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

EBTNodeResult::Type UBTTask_MoveToCircular::PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::PerformMoveTask(OwnerComp, NodeMemory);
}

UAITask_MoveTo* UBTTask_MoveToCircular::PrepareMoveTask(UBehaviorTreeComponent& OwnerComp, UAITask_MoveTo* ExistingTask,
	FAIMoveRequest& MoveRequest)
{
	return Super::PrepareMoveTask(OwnerComp, ExistingTask, MoveRequest);
}
