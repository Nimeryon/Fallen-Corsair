// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

// Sets default values for this component's properties
UGun::UGun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_maxGunAmmo = 3;
	m_gunAmmo = m_maxGunAmmo;
	m_distance = 2000.f;
	m_ammoCost = 1;
	m_superAmmoCost = m_maxGunAmmo;
	// ...
}


// Called when the game starts
void UGun::BeginPlay()
{
	Super::BeginPlay();

	if(m_gunAmmo > m_maxGunAmmo)
		m_gunAmmo = m_maxGunAmmo;
	// ...
	
}


// Called every frame
void UGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UGun::GetAmmo()
{
	return m_gunAmmo;
}
void UGun::SetAmmo(int newAmmo)
{
	m_gunAmmo = newAmmo;
}

void UGun::Shoot()
{
	if(m_gunAmmo > 0)
	{
		FHitResult outHit;
		/// change the owner by the owner camera
		FVector start = GetOwner()->GetActorLocation();
		FVector end = GetOwner()->GetActorForwardVector() * m_distance + start;

		/// trace
		GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility);
		/// spawn projectile here
		FActorSpawnParameters SpawnParameters;
		if(m_bullet)
		{
			//GetWorld()->SpawnActor(m_bullet, start, GetOwner()->GetActorRotation(), SpawnParameters);
		}
		UE_LOG(LogTemp, Warning, TEXT("Shoot"));
		m_gunAmmo -= m_ammoCost;
	}
}

void UGun::SuperShoot()
{
	/// special AOE attack
}

