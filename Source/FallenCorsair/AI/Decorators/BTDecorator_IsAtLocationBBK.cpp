// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsAtLocationBBK.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_IsAtLocationBBK::UBTDecorator_IsAtLocationBBK(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Is At Location Blackboard Key";
	bCreateNodeInstance = true;
	
	BBK_MinRadius.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsAtLocationBBK, BBK_MinRadius));
	BBK_MaxRadius.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsAtLocationBBK, BBK_MaxRadius));
}

void UBTDecorator_IsAtLocationBBK::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
	Super::OnInstanceCreated(OwnerComp);
	
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	AcceptableRadius = UKismetMathLibrary::RandomFloatInRange(
		BlackBoard->GetValueAsFloat(BBK_MinRadius.SelectedKeyName),
		BlackBoard->GetValueAsFloat(BBK_MaxRadius.SelectedKeyName)
	);
}
