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
	m_maxSoul = 100 * m_maxSlot;
	
}


// Called every frame
void UBarrel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(m_slot < m_maxSlot)
	{
		m_currentSoul = m_currentSoul + ((m_speedCharge / 100)  * m_maxSoul * DeltaTime);
		int soul = m_currentSoul;

		if(m_currentSoul >= (m_slot + 1) * 100)
		{
			m_slot++;
		}
		
		if(m_currentSoul >= m_maxSoul)
		{
			m_currentSoul = m_maxSoul;
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
	m_currentSoul = newSlot * 100;
}
int UBarrel::GetMaxSlot()
{
	return m_maxSlot;
}

void UBarrel::CollectSoul()
{
	//float soul = m_maxSlot * 100;
	
	
	
	int slotToAdd = 0;
	for(int i = 0; i < slotToAdd; i++)
	{
		if(m_ennemyDropSoul >= 100)
		{
			m_ennemyDropSoul = m_ennemyDropSoul - 100;
			slotToAdd++;
		}
	}
	m_slot = FMath::Clamp(m_slot + slotToAdd,0,m_maxSlot);
	m_currentSoul = m_currentSoul + (m_ennemyDropSoul / 100);
}