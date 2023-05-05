// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomPointInRadius.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_GetRandomPointInRadius::UBTTask_GetRandomPointInRadius(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Get Next Random Point To";
	INIT_TASK_NODE_NOTIFY_FLAGS();

	bCreateNodeInstance = true;

	// Only actors for Target
	BBKTarget.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetRandomPointInRadius, BBKTarget), AActor::StaticClass());

	// Only vectors for TargetPosition
	BBKTargetPosition.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetRandomPointInRadius, BBKTargetPosition));
}

void UBTTask_GetRandomPointInRadius::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);

	if (MaxDistance <= MinDistance)
		MaxDistance = MinDistance;
}

EBTNodeResult::Type UBTTask_GetRandomPointInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const float Distance = UKismetMathLibrary::RandomFloatInRange(MinDistance, MaxDistance);
	const FVector Direction = OwnerComp.GetAIOwner()->GetPawn()->GetActorForwardVector().GetSafeNormal2D();
	const FVector RandomDirection = Direction.RotateAngleAxis(UKismetMathLibrary::RandomFloatInRange(0, 360), FVector::UpVector);
	FVector Position;
	
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();

	if (GenerateAroundTarget)
	{
		AActor* TargetActor = Cast<AActor>(BlackBoard->GetValue<UBlackboardKeyType_Object>(BBKTarget.SelectedKeyName));
		if (TargetActor)
			Position = TargetActor->GetActorLocation() + RandomDirection * Distance;
	}
	else Position = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation() + RandomDirection * Distance;
	
	FNavLocation NavPosition;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	NavSys->ProjectPointToNavigation(Position, NavPosition, FVector::ZeroVector);
	
	BlackBoard->SetValueAsVector(BBKTargetPosition.SelectedKeyName, Position);
	if (DrawDebugPoint)
		UKismetSystemLibrary::DrawDebugPoint(GetWorld(), NavPosition.Location, 8, FLinearColor::Red, 2);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
