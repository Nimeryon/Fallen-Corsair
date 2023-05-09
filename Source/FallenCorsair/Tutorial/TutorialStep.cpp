// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialStep.h"

#include "TutorialActor.h"
#include "FallenCorsair/FallenCorsairCharacter.h"
#include "FallenCorsair/Components/Melee.h"

void UTutorialStep::OnStepComplete()
{
	m_bIsActive = false;
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Yellow, FString::Printf(TEXT("%s: step complete"), *m_name));
#endif

	OnStepCompleteBP();
}

void UTutorialStep::OnStepCompleteBP_Implementation()
{
}

void UTutorialStep::OnStepBegin()
{
	m_bIsActive = true;
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Yellow, m_name + ": step begin");
#endif

	OnStepBeginBP();
}

void UTutorialStep::OnStepBeginBP_Implementation()
{
}

void UTutorialStep::Initialize(class ATutorialActor* tutorial, AFallenCorsairCharacter* player)
{
	if(tutorial)
	{
		m_tutorial = tutorial;
	}
	
	if(GetOuter())
	{
		if(!player)
			return;

		m_player = player;
		
		switch (m_actionPerformed)
		{
		case ETutorialActionType::LightAttack:
			player->MeleeComponent->DeleguateMeleeSoft.AddDynamic(this, &UTutorialStep::CheckStepComplete);
			break;
		case ETutorialActionType::HeavyAttack:
			player->MeleeComponent->DeleguateMeleeHeavy.AddDynamic(this, &UTutorialStep::CheckStepComplete);
			break;
		case ETutorialActionType::SoulPickup:
			//player->OnSoulPickup.AddDynamic(this, &UTutorialStep::CheckStepComplete);
			break;
		case ETutorialActionType::Shoot:
			player->OnShoot.AddDynamic(this, &UTutorialStep::CheckStepComplete);
			break;
		case ETutorialActionType::Dodge:
			player->OnDodge.AddDynamic(this, &UTutorialStep::CheckStepComplete);
			break;
		default:
			break;
		}
	}
}

void UTutorialStep::CheckStepComplete()
{
	if(m_bIsActive)
	{
		m_count++;
		if (m_count >= m_goal)
		{
			m_tutorial->PlayNextStep();
		}
	}
}
