// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryConditionBase.h"

#include "WaveTracker.h"
#include "FallenCorsair/FallenCorsairCharacter.h"
#include "FallenCorsair/Components/Melee.h"
#include "FallenCorsair/Enemies/AlienBase.h"

// Sets default values for this component's properties
UVictoryConditionBase::UVictoryConditionBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UVictoryConditionBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_waveTracker = GetOwner()->FindComponentByClass<UWaveTracker>();

	AFallenCorsairCharacter* player = Cast<AFallenCorsairCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	player->OnShoot.AddDynamic(this, &UVictoryConditionBase::OnShootPerformed);
	player->MeleeComponent->DeleguateMeleeSoft.AddDynamic(this, &UVictoryConditionBase::OnLightAttackPerformed);
	player->MeleeComponent->DeleguateMeleeHeavy.AddDynamic(this, &UVictoryConditionBase::OnHeavyAttackPerformed);
	
	for (auto& VictoryCondition : m_victoryConditions)
	{
		if (VictoryCondition.ConditionType == EConditionType::EnemiesRemaining)
		{
			m_killAll = true;
			VictoryCondition.bIsConditionMet = true;
		}
	}
}

void UVictoryConditionBase::CheckConditions()
{
	if(!m_bAllConditionsMet || m_killAll)
	{
		m_bAllConditionsMet = true;
		if(m_victoryConditions.IsEmpty())
		{
			return;
		}
		for (auto& VictoryCondition : m_victoryConditions)
		{
			if(!VictoryCondition.bIsConditionMet)
			{
				if (VictoryCondition.Count >= VictoryCondition.Goal)
				{
					VictoryCondition.bIsConditionMet = true; 
				}
				else 
				{
					m_bAllConditionsMet = false;
				}
			}
		}
		if (m_bAllConditionsMet)
		{
			if(m_killAll)
			{
				m_waveTracker->m_maxEnemies = 0;
				m_waveTracker->m_minEnemies = 0;
				if (m_waveTracker->m_enemiesAlive == 0)
					m_killAll = false;
			}

			m_waveTracker->m_bCanSpawnEnemies = false;
			OnZoneVictory.Broadcast();
		}
	}
}

void UVictoryConditionBase::OnLightAttackPerformed()
{
	for (auto& VictoryCondition : m_victoryConditions)
	{
		if (
			VictoryCondition.ConditionType == EConditionType::ActionPerformed
			&& VictoryCondition.ActionPerformed == EActionType::LightAttack
			)
		{
			VictoryCondition.Count++;
		}
	}
	CheckConditions();
}

void UVictoryConditionBase::OnHeavyAttackPerformed()
{
	for (auto& VictoryCondition : m_victoryConditions)
	{
		if (
			VictoryCondition.ConditionType == EConditionType::ActionPerformed
			&& VictoryCondition.ActionPerformed == EActionType::HeavyAttack
			)
		{
			VictoryCondition.Count++;
		}
	}
	CheckConditions();
}

void UVictoryConditionBase::OnShootPerformed()
{
	for (auto& VictoryCondition : m_victoryConditions)
	{
		if (
			VictoryCondition.ConditionType == EConditionType::ActionPerformed
			&& VictoryCondition.ActionPerformed == EActionType::Shoot
			)
		{
			VictoryCondition.Count++;
		}
	}
	CheckConditions();
}

void UVictoryConditionBase::OnDodgePerformed()
{
	for (auto& VictoryCondition : m_victoryConditions)
	{
		if (
			VictoryCondition.ConditionType == EConditionType::ActionPerformed
			&& VictoryCondition.ActionPerformed == EActionType::Dodge
			)
		{
			VictoryCondition.Count++;
		}
	}
	CheckConditions();
}

void UVictoryConditionBase::OnEnemyDeath(AAlienBase* Alien)
{
	if (!Alien) return;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Alien->GetName());
	
	for ( auto& VictoryCondition : m_victoryConditions )
	{
		if (
			VictoryCondition.ConditionType == EConditionType::EnemiesKilled
			&& Alien->IsA(VictoryCondition.AlienType)
			)
		{
			VictoryCondition.Count++;
		}
	}
	CheckConditions();
}

void UVictoryConditionBase::OnWaveOver()
{
	for (auto& VictoryCondition : m_victoryConditions)
	{
		if ( VictoryCondition.ConditionType == EConditionType::WavesCleared )
		{
			VictoryCondition.Count++;
		}
	}
	CheckConditions();
}