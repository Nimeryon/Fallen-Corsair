// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialActor.h"

#include "TutorialStep.h"
#include "FallenCorsair/FallenCorsairCharacter.h"

// Sets default values
ATutorialActor::ATutorialActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATutorialActor::BeginPlay()
{
	Super::BeginPlay();

	m_player = Cast<AFallenCorsairCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	for (auto step : m_tutorialSteps)
	{
		if(step.GetDefaultObject())
			step.GetDefaultObject()->Initialize(this, m_player);
	}
	
	PlayNextStep(true);
}

void ATutorialActor::TutorialComplete()
{
	m_bIsTutorialComplete = true;
	OnTutorialComplete.Broadcast();
}

void ATutorialActor::PlayNextStep(bool isFirstStep)
{
	if(!isFirstStep)
	{
	
		UTutorialStep* step = m_tutorialSteps[m_currentStepIndex].GetDefaultObject();
		if(step)
		{
			step->OnStepComplete();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tutorial next step is not valid"));
		}
	
		m_currentStepIndex++;
	}
	
	if (m_currentStepIndex >= m_tutorialSteps.Num())
	{
		TutorialComplete();
		return;
	}
	
	UTutorialStep* step = m_tutorialSteps[m_currentStepIndex].GetDefaultObject();
	
	if(step)
	{
		m_currentStepName = step->m_name;
		step->OnStepBegin();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tutorial step is not valid"));
	}
}

