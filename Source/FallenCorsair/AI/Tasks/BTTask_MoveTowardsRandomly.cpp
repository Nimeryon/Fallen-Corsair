// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveTowardsRandomly.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "AISystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"

void UBTTask_MoveTowardsRandomly::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);

	if (MaxSegmentLength <= MinSegmentLength)
		MaxSegmentLength = MinSegmentLength;
}

void UBTTask_MoveTowardsRandomly::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	
}

EBTNodeResult::Type UBTTask_MoveTowardsRandomly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	if (OwnerComp.GetOwner()->GetActorLocation().Equals(GetTargetPosition(OwnerComp), AcceptableRadius))
	{
		NodeResult = EBTNodeResult::Succeeded;
	}
	
	return NodeResult;
}

FVector UBTTask_MoveTowardsRandomly::GetNextRandomPosition(const AActor* Actor, const FVector& TargetPosition)
{
	const float DistanceToPoint = FVector::Distance(Actor->GetActorLocation(), TargetPosition);
	
	if (DistanceToPoint < MaxSegmentLength)
	{
		return TargetPosition;
	}

	FVector Direction = TargetPosition - Actor->GetActorLocation().GetSafeNormal2D();
	const float DirectionAngle = UKismetMathLibrary::Atan2(Direction.Y, Direction.X);
	const float RandomAngle = UKismetMathLibrary::DegreesToRadians(AngleRange * UKismetMathLibrary::RandomFloat() * 2.f - 1.f);
	const float Angle = DirectionAngle + RandomAngle;
	Direction = Direction.RotateAngleAxis(Angle, FVector::UpVector);
	const float Length = MinSegmentLength + (MaxSegmentLength - MinSegmentLength) * UKismetMathLibrary::RandomFloat();

    const FVector Position = Actor->GetActorLocation() + Direction * Length;
	UKismetSystemLibrary::DrawDebugPoint(GetWorld(), Position, 8, FLinearColor::Red, 1);

	return  Actor->GetActorLocation() + Direction * Length;
}

FVector UBTTask_MoveTowardsRandomly::GetTargetPosition(UBehaviorTreeComponent& OwnerComp)
{
	const UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = BlackBoard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* TargetActor = Cast<AActor>(KeyValue);
		if (TargetActor)
		{
			return TargetActor->GetActorLocation();
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		return BlackBoard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
	}

	return OwnerComp.GetOwner()->GetActorLocation();
}
