// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundAlien.h"
#include "FallenCorsair/Components/Melee.h"

AGroundAlien::AGroundAlien(const FObjectInitializer& ObjectInitializer)
{
	m_meleeComponent = CreateDefaultSubobject<UMelee>(TEXT("Melee Component"));
}

void AGroundAlien::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

bool AGroundAlien::Attack(AActor* Target)
{
	return true;
}

AlienState AGroundAlien::GetState() const { return m_state; }

void AGroundAlien::SetState(AlienState State) { m_state = State; }