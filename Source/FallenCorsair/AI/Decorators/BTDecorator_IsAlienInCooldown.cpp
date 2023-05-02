// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsAlienInCooldown.h"

#include "AIController.h"
#include "FallenCorsair/Enemies/AlienBase.h"
#include "FallenCorsair/Enemies/GroundAlien.h"

bool UBTDecorator_IsAlienInCooldown::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AGroundAlien* Alien = Cast<AGroundAlien>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Alien) return false;
	return Alien->IsInCooldown();
}
