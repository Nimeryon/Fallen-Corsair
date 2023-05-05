// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetAngledRandomTo.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_GetAngledRandomTo::UBTTask_GetAngledRandomTo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Get Next Random Point To";
	INIT_TASK_NODE_NOTIFY_FLAGS();

	bCreateNodeInstance = true;

	// Only actors for Target
	BBKTarget.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetAngledRandomTo, BBKTarget), AActor::StaticClass());

	// Only vectors for TargetPosition
	BBKTargetPosition.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetAngledRandomTo, BBKTargetPosition));
}

void UBTTask_GetAngledRandomTo::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);

	if (MaxSegmentLength <= MinSegmentLength)
		MaxSegmentLength = MinSegmentLength;
}

EBTNodeResult::Type UBTTask_GetAngledRandomTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
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
	const FVector Position = GetNextRandomPosition(OwnerComp.GetAIOwner()->GetPawn(), TargetPosition);
	BlackBoard->SetValueAsVector(BBKTargetPosition.SelectedKeyName, Position);

	return EBTNodeResult::Succeeded;
}

FVector UBTTask_GetAngledRandomTo::GetNextRandomPosition(const AActor* Actor, const FVector& TargetPosition)
{
	/* const float DistanceToPoint = FVector::Distance(Actor->GetActorLocation(), TargetPosition);
	if (DistanceToPoint < MaxSegmentLength)
	{
		return TargetPosition;
	} */

	const FVector Direction = (TargetPosition - Actor->GetActorLocation()).GetSafeNormal2D();
	const float RandomAngle = UKismetMathLibrary::RandomFloatInRange(-AngleRange, AngleRange);
	const FVector RandomAngledDirection = Direction.RotateAngleAxis(RandomAngle, FVector::UpVector);
	const float Length = UKismetMathLibrary::RandomFloatInRange(MinSegmentLength, MaxSegmentLength);

	const FVector Position = Actor->GetActorLocation() + RandomAngledDirection * Length;

	FNavLocation NavPosition;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	NavSys->ProjectPointToNavigation(Position, NavPosition, FVector::ZeroVector);

	if (DrawDebugPoint)
		UKismetSystemLibrary::DrawDebugPoint(GetWorld(), NavPosition.Location, 8, FLinearColor::Red, 2);
	
	return NavPosition.Location;
}