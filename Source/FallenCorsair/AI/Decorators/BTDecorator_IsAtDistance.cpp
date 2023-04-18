// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsAtDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTDecorator_IsAtDistance::UBTDecorator_IsAtDistance(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Is At Distance";
	bCreateNodeInstance = true;
	
	// Only actors and vectors for Target
	BBKTarget.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsAtDistance, BBKTarget), AActor::StaticClass());

}

bool UBTDecorator_IsAtDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                           uint8* NodeMemory) const
{
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
		return false;
	}
	
	const float TargetDistance = FVector::Distance(TargetPosition, OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());
	return TargetDistance <= Distance;
}
