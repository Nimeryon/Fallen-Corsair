// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrel.h"

// Sets default values for this component's properties
UBarrel::UBarrel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBarrel::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBarrel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(m_slot < m_maxSlot)
	{
		m_currentSoul = m_currentSoul + (1 / m_speedCharge) * DeltaTime;
		if(m_currentSoul >= m_maxSoul)
		{
			m_currentSoul = 0;
			m_slot++;
		}
	}
	
}

int UBarrel::GetSlot()
{
	return m_slot;
}
void UBarrel::SetSlot(int newSlot)
{
	m_slot = newSlot;
}

int UBarrel::GetMaxSlot()
{
	return m_maxSlot;
}

void UBarrel::CollectSoul()
{
	m_currentSoul = m_currentSoul + m_ennemyDropSoul;
}

