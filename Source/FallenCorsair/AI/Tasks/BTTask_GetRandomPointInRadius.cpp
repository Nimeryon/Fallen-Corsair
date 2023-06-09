// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomPointInRadius.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
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
	FVector Position;
	
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	ACharacter* Actor = OwnerComp.GetAIOwner()->GetCharacter();
	AActor* TargetActor = Cast<AActor>(BlackBoard->GetValue<UBlackboardKeyType_Object>(BBKTarget.SelectedKeyName));

	Position = GetRandomPositionInRadius(Actor, TargetActor, NavSys, Distance, Direction, Position, MaxIteration);
	BlackBoard->SetValueAsVector(BBKTargetPosition.SelectedKeyName, Position);
	if (DrawDebugPoint)
		UKismetSystemLibrary::DrawDebugPoint(GetWorld(), Position, 8, FLinearColor::Red, 2);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

FVector UBTTask_GetRandomPointInRadius::GetRandomPositionInRadius(
	ACharacter* Character,
	AActor* Target,
	UNavigationSystemV1* NavSys,
	float Distance,
	FVector Direction,
	FVector& Position,
	int Iteration
)
{
	const FVector RandomDirection = Direction.RotateAngleAxis(UKismetMathLibrary::RandomFloatInRange(0, 360), FVector::UpVector);
	if (GenerateAroundTarget)
	{
		if (Target)
			Position = Target->GetActorLocation() + RandomDirection * Distance;
	}
	else Position = Character->GetActorLocation() + RandomDirection * Distance;
	
	FNavLocation NavPosition;
	NavSys->ProjectPointToNavigation(Position, NavPosition, FVector::ZeroVector);

	TArray<FHitResult> Hits;
	const  TArray<AActor*> ActorsToIgnore = {};
	UKismetSystemLibrary::CapsuleTraceMultiByProfile(
		GetWorld(),
		Character->GetActorLocation(),
		NavPosition.Location,
		Character->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.f,
		Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
		"AlienAttack",
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hits,
		true
	);

	for (FHitResult& Hit : Hits)
	{
		if (Hit.GetActor() == Target)
		{
			if (Iteration <= 0)
				return NavPosition.Location;
			
			return GetRandomPositionInRadius(Character, Target, NavSys, Distance, Direction, Position, --Iteration);
		}
	}
	
	return NavPosition.Location;
}