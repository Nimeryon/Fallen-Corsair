// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsAlienStunned.h"

#include "AIController.h"
#include "FallenCorsair/Enemies/AlienBase.h"

bool UBTDecorator_IsAlienStunned::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAlienBase* Alien = Cast<AAlienBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Alien) return false;
	return Alien->IsStunned();
}
