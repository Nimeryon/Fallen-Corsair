// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_GetNextRandomPointTo.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTaskNode_GetNextRandomPointTo::UBTTaskNode_GetNextRandomPointTo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Get Next Random Point To";
	INIT_TASK_NODE_NOTIFY_FLAGS();

	bCreateNodeInstance = true;

	// Only actors and vectors for Target
	BBKTarget.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_GetNextRandomPointTo, BBKTarget), AActor::StaticClass());

	// Only vectors for TargetPosition
	BBKTargetPosition.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_GetNextRandomPointTo, BBKTargetPosition));
}

void UBTTaskNode_GetNextRandomPointTo::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);

	if (MaxSegmentLength <= MinSegmentLength)
		MaxSegmentLength = MinSegmentLength;
}

EBTNodeResult::Type UBTTaskNode_GetNextRandomPointTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// Get Target Position
	FVector TargetPosition;
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BlackBoard->GetValue<UBlackboardKeyType_Object>(BBKTarget.SelectedKeyName));
	if (TargetActor)
	{
		TargetPosition = TargetActor->GetActorLocation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target actor was nullptr."));
		return EBTNodeResult::Failed;
	}

	// Get Next Target Position
	const FVector Position = GetNextRandomPosition(OwnerComp.GetOwner(), TargetPosition);
	BlackBoard->SetValueAsVector(BBKTargetPosition.SelectedKeyName, Position);

	return EBTNodeResult::Succeeded;
}

FVector UBTTaskNode_GetNextRandomPointTo::GetNextRandomPosition(const AActor* Actor, const FVector& TargetPosition)
{
	const float DistanceToPoint = FVector::Distance(Actor->GetActorLocation(), TargetPosition);
	
	if (DistanceToPoint < MaxSegmentLength)
	{
		return TargetPosition;
	}

	const FVector Direction = TargetPosition - Actor->GetActorLocation();
	const float RandomAngle = AngleRange * (UKismetMathLibrary::RandomFloat() * 2.f - 1.f);
	const FVector RandomAngledDirection = Direction.RotateAngleAxis(RandomAngle, FVector::UpVector).GetSafeNormal2D();
	
	const float Length = MinSegmentLength + (MaxSegmentLength - MinSegmentLength) * UKismetMathLibrary::RandomFloat();

	const FVector Position = Actor->GetActorLocation() + RandomAngledDirection * Length;

	FNavLocation NavPosition;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	NavSys->ProjectPointToNavigation(Position, NavPosition, FVector::ZeroVector);
	
	UKismetSystemLibrary::DrawDebugPoint(GetWorld(), NavPosition.Location, 8, FLinearColor::Red, 10);
	
	return NavPosition.Location;
}