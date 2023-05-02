// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Barrel.h"
#include "Bullet.h"
#include "../FallenCorsairCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetStringLibrary.h"

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
		if(m_ownerRef->GetMesh()->DoesSocketExist(m_socketLoc))
		{
			start = m_ownerRef->GetMesh()->GetSocketLocation(m_socketLoc);
		}
		
		FVector end = m_ownerRef->GetFollowCamera()->GetForwardVector() * m_distance + start;

		/// trace
		GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility);

		FActorSpawnParameters SpawnInfo;
		//SpawnInfo.
		if(m_bullet)
		{
			m_spawnBullet = GetWorld()->SpawnActor<ABullet>(m_bullet, start, m_ownerRef->GetFollowCamera()->GetComponentRotation(), SpawnInfo);
			m_spawnBullet->SetBulletSetting(m_bulletSpeed,m_bulletDammage, m_explosionDammage, m_bulletExplosionRadius, m_explosionDuration, m_bulletLifeSpan, m_bulletRadius, m_ChargeSpeed, m_ownerRef);
			
		}
		// UE_LOG(LogTemp, Warning, TEXT("Shoot"));
	}
}

void UGun::StopCharge(bool bIsCancel)
{


	if(m_spawnBullet && !m_spawnBullet->GetIsBulletCharge() || bIsCancel)
	{
		if(m_spawnBullet && !m_spawnBullet->GetIsBulletLaunch())
		{
			m_spawnBullet->StopChargeSound();
			m_spawnBullet->Destroy();
			m_spawnBullet = nullptr;
		}
			
	}
	else if(m_spawnBullet->GetIsBulletCharge())
	{
		m_gunAmmo -= m_ammoCost;
		m_barrelRef->SetSlot(m_gunAmmo);
		m_spawnBullet->StopChargeSound();
		m_spawnBullet->LaunchBullet(GetDirectionBullet());
		m_spawnBullet = nullptr;
	}
}

FVector UGun::GetDirectionBullet()
{
	FVector ImpactLocation = GetImpactPointOnScreen();

	if (ImpactLocation == FVector::Zero())
		return FVector::Zero();

	FVector Dir = ImpactLocation - m_ownerRef->GetMesh()->GetSocketLocation(m_socketLoc);;
	Dir.Normalize();
	return Dir;
}

// Private
UCameraComponent* UGun::GetParentCamera()
{
	// Récupérer l'acteur parent
	AActor* ParentActor = GetOwner();
	
	if (!ParentActor)
		return nullptr;

	UCameraComponent* ParentCamera = ParentActor->FindComponentByClass<UCameraComponent>();

	return ParentCamera;
}

FVector UGun::GetImpactPointOnScreen()
{
	
	UCameraComponent* ParentCamera = GetParentCamera();

	if (!ParentCamera)
		return FVector::Zero();

	FHitResult HitResult;
	FVector StartLocation = ParentCamera->GetComponentLocation();
	FVector EndLocation =  ParentCamera->GetComponentLocation() + ParentCamera->GetForwardVector() * 10000;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	if (bHit)
		return FVector(HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, HitResult.ImpactPoint.Z);

	return FVector::Zero();
}
