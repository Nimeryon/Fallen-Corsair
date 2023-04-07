// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Barrel.h"
#include "Bullet.h"
#include "FallenCorsairCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values for this component's properties
UGun::UGun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UGun::BeginPlay()
{
	Super::BeginPlay();

	//if(m_gunAmmo > m_maxGunAmmo)
	//	m_gunAmmo = m_maxGunAmmo;

	m_ownerRef = Cast<AFallenCorsairCharacter>(GetOwner());
	
	m_barrelRef = m_ownerRef->barrelComp;

	m_gunAmmo = m_barrelRef->GetSlot();
	m_maxGunAmmo = m_barrelRef->GetMaxSlot();
	
	/// recup la ref du barrel et set les valeurs des ammos par rapport au slot du barrel
	
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
	m_gunAmmo = m_barrelRef->GetSlot();
	if(m_gunAmmo > 0 && m_ownerRef)
	{
		FHitResult outHit;
		/// change the owner by the owner camera

		/// Change start by the location of the socket gun mesh
		FVector start = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100;
		//m_ownerRef->GetMesh()->GetSocketLocation(""); 
		FVector end = m_ownerRef->GetFollowCamera()->GetForwardVector() * m_distance + start;

		/// trace
		GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility);

		FActorSpawnParameters SpawnInfo;
		if(m_bullet)
		{
			/// spawn bullet, rotation base on camera rotation
			GetWorld()->SpawnActor<ABullet>(m_bullet, start, m_ownerRef->GetFollowCamera()->GetComponentRotation(), SpawnInfo)
			->SetBulletSetting(m_bulletSpeed,m_bulletDammage, m_bulletDammageRadius, m_bulletLifeSpan, m_bulletRadius);
		}
		UE_LOG(LogTemp, Warning, TEXT("Shoot"));
		m_gunAmmo -= m_ammoCost;
		m_barrelRef->SetSlot(m_gunAmmo);
	}
}

void UGun::SuperShoot()
{
	/// special AOE attack
}