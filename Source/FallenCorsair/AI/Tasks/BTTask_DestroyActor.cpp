// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DestroyActor.h"

#include "AIController.h"

void UBTTask_DestroyActor::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

EBTNodeResult::Type UBTTask_DestroyActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->GetPawn()->Destroy();
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}