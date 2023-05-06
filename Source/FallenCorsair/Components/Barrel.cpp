// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrel.h"
#include "NiagaraFunctionLibrary.h"
#include "../FallenCorsairCharacter.h"

#include "Kismet/KismetMaterialLibrary.h"

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
	m_ownerRef = Cast<AFallenCorsairCharacter>(GetOwner());
	m_maxSoul = 100 * m_maxSlot;
	m_currentSoul = 100 * m_slot;
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_collection, "Slot", m_maxSlot);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_collection, "GaugePercent", m_maxSoul / m_maxSlot);
}


// Called every frame
void UBarrel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(m_slot < m_maxSlot)
	{
		m_currentSoul = m_currentSoul + ((m_speedCharge / 100)  * m_maxSoul * DeltaTime);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), m_collection, "GaugePercent",m_currentSoul / m_maxSlot);

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
	if(m_sparkParticle && m_ownerRef->GetMesh()->DoesSocketExist(m_socketLoc))
	{
		FVector SpawnLocation = m_ownerRef->GetMesh()->GetSocketLocation(m_socketLoc);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		UNiagaraFunctionLibrary::SpawnSystemAttached(m_sparkParticle, m_ownerRef->GetRootComponent(), m_socketLoc, SpawnLocation, SpawnRotation,EAttachLocation::KeepWorldPosition, true, true);
		
	}
	
	m_slot = m_slot - newSlot;
	m_currentSoul = m_currentSoul - newSlot * 100;
}
int UBarrel::GetMaxSlot()
{
	return m_maxSlot;
}

void UBarrel::CollectSoul()
{
	m_currentSoul = FMath::Clamp(m_currentSoul + ((m_ennemyDropSoul / 100)  * m_maxSoul), m_currentSoul, m_maxSoul);
}