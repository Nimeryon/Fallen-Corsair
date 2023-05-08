// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_IsAtLocation.h"
#include "BTDecorator_IsAtLocationBBK.generated.h"

/**
 * 
 */
UCLASS()
class FALLENCORSAIR_API UBTDecorator_IsAtLocationBBK : public UBTDecorator_IsAtLocation
{
	GENERATED_BODY()
	
	UBTDecorator_IsAtLocationBBK(const FObjectInitializer& ObjectInitializer);
	
	virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp) override;
	
protected:
	/** Min Acceptable radius */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BBK_MinRadius;
	
	/** Max Acceptable radius */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BBK_MaxRadius;
};
