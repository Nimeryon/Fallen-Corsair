// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

// Sets default values for this component's properties
UGun::UGun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_maxGunCharge = 3;
	m_gunCharge = m_maxGunCharge;
	// ...
}


// Called when the game starts
void UGun::BeginPlay()
{
	Super::BeginPlay();

	if(m_gunCharge > m_maxGunCharge)
		m_gunCharge = m_maxGunCharge;
	// ...
	
}


// Called every frame
void UGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UGun::GetCharge()
{
	return m_gunCharge;
}

void UGun::SetCharge(int newCharge)
{
	m_gunCharge = newCharge;
}

void UGun::Fire()
{
	if(m_gunCharge == m_maxGunCharge)
	{
		SuperFire();
		m_gunCharge = 0;
	}
	
	if(m_gunCharge > 0)
	{
		FHitResult outHit;
		/// change the owner by the owner camera
		FVector start = GetOwner()->GetActorLocation();
		FVector end = GetOwner()->GetActorForwardVector() * m_distance + start;

		/// trace
		GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility);
		/// spawn projectile here
		
		m_gunCharge--;
	}
}

void UGun::SuperFire()
{
	/// special AOE attack
}

