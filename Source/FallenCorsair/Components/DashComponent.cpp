// Fill out your copyright notice in the Description page of Project Settings.


#include "DashComponent.h"

#include "FallenCorsair/FallenCorsairCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Barrel.h"

// Sets default values for this component's properties
UDashComponent::UDashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDashComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	m_ownerRef = Cast<AFallenCorsairCharacter>(GetOwner());
	m_barrelRef = m_ownerRef->barrelComp;
	
}

// Called every frame
void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (m_bCanPerfectDodge)
	{
		m_currentPerfectDodgeTime += DeltaTime;
		if (m_currentPerfectDodgeTime >= m_perfectDodgeTime)
		{
			m_bCanPerfectDodge = false;
			m_currentPerfectDodgeTime = 0;
		}
	}

	if(m_bIsEdge)
	{
		DetectEdge();
		if(m_bIsDashing)
		{
			float CurrentDistance = (m_ownerRef->GetActorLocation() - m_startLoc).Length() + 5;
			if(CurrentDistance <= m_actualDistance)
			{
				m_ownerRef->GetCharacterMovement()->MaxWalkSpeed = m_maxDistance / m_dashTime * m_DashCurve->GetFloatValue(CurrentDistance / m_actualDistance);
				m_ownerRef->GetCharacterMovement()->Velocity = (m_newLoc - m_startLoc).GetSafeNormal() * m_ownerRef->GetCharacterMovement()->MaxWalkSpeed;
				m_ownerRef->GetCharacterMovement()->AddInputVector((m_newLoc - m_startLoc).GetSafeNormal(), true);
				
				ImpulseEnnemy();

				if(CurrentDistance / m_actualDistance > m_invicibleTimePercent / 100)
				{
					m_ownerRef->SetCanBeDamage(true);
					// Not Invincible
					if(CurrentDistance / m_actualDistance > m_slowMoTimePercent / 100 && m_bIsPerfectDodge)
					{
						m_bIsSlowMo = true;
						m_ownerRef->EnableInput(nullptr);
						UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1 / m_slowMoRate);
						m_ownerRef->CustomTimeDilation = m_slowMoRate;
					}
				}
				else if(m_bHasIFrames)
				{
					// Invincible
					// set a bool cant take damage
					m_ownerRef->SetCanBeDamage(false);
				}
			}
			else
			{
				m_ownerRef->EnableInput(nullptr);
				StopDash();
			}
		}
	}
	else 
	{
		m_ownerRef->EnableInput(nullptr);
	}
}

void UDashComponent::DashPressed()
{
	if(m_barrelRef->GetSlot() > m_energyCost && m_DashCurve)
	{
		if(m_bCanDash && !m_bIsSlowMo)
		{
			PerformDash();
			m_ownerRef->DisableInput(nullptr);
			m_bCanDash = false;
			GetWorld()->GetTimerManager().SetTimer(m_dashTimer, this, &UDashComponent::DashCD, m_dashCD);

			m_barrelRef->SetSlot(m_barrelRef->GetSlot() - m_energyCost);
		}
	}
}

void UDashComponent::PerformDash()
{
	m_startLoc = m_ownerRef->GetActorLocation();
	m_newLoc = m_startLoc + (m_ownerRef->GetActorForwardVector() * m_maxDistance);
	const float CapsuleRadius = m_ownerRef->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const float CapsuleHalfHeight = m_ownerRef->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FHitResult Hit;

	UKismetSystemLibrary::CapsuleTraceSingleByProfile(GetWorld(), m_startLoc, m_newLoc, CapsuleRadius, CapsuleHalfHeight, TEXT("Visibility"), false, m_ownerRef->SetIgnoreCharacterActors(), EDrawDebugTrace::None, Hit, true);

	if(Hit.bBlockingHit)
	{
		if(FVector::DotProduct(Hit.Normal, (m_startLoc - Hit.Location).GetSafeNormal()) > 90 / m_maxAngleSlope)
			m_newLoc = Hit.Location;
		else
			m_newLoc = Hit.TraceEnd;
	}
	else
	{
		m_newLoc = Hit.TraceEnd;
	}

	m_bIsEdge = true;

	// if (m_bCanPerfectDodge)
	//	PerfectDodge();
}

void UDashComponent::StartDash()
{
	m_actualDistance = (m_newLoc - m_startLoc).Length();
	m_bIsDashing = true;

	FVector startTrace = m_ownerRef->GetActorLocation();
	FVector endTrace = startTrace + (m_ownerRef->GetActorForwardVector() * 50);
	FHitResult Hit;

	
	UKismetSystemLibrary::SphereTraceSingleByProfile(GetWorld(), startTrace, endTrace, 30.f, TEXT("Visibility"), false, m_ownerRef->SetIgnoreCharacterActors(), EDrawDebugTrace::None, Hit, true);
	
	if(Hit.bBlockingHit && FVector::DotProduct(Hit.Normal, (Hit.TraceStart - Hit.Location).GetSafeNormal()) < 1)
	{
		StopDash();
	}
}

void UDashComponent::StopDash()
{
	m_bIsDashing = false;
	m_bIsEdge = false;
	m_ownerRef->GetCharacterMovement()->MaxWalkSpeed = 500;
	m_ownerRef->GetCharacterMovement()->Velocity = FVector(0,0,0);
	m_ownerRef->GetCharacterMovement()->StopMovementImmediately();

	GetWorld()->GetTimerManager().SetTimer(m_slowMoTimer, this, &UDashComponent::DashSlowMoReset, UGameplayStatics::GetGlobalTimeDilation(GetWorld()) * m_slowMoDuration);
}

void UDashComponent::DetectEdge()
{
	FVector startTrace = m_ownerRef->GetActorLocation() + ((m_newLoc - m_ownerRef->GetActorLocation()).GetSafeNormal() * 50);
	FVector endTrace  =( m_ownerRef->GetActorLocation() + (m_ownerRef->GetActorForwardVector() * 100)) - FVector(0,0, m_groundDistance);
	FHitResult Hit;

	UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), startTrace, endTrace, TEXT("Visibility"), false,  m_ownerRef->SetIgnoreCharacterActors(), EDrawDebugTrace::None, Hit, true);

	if(Hit.bBlockingHit)
		StartDash();
	else
		StopDash();
}

void UDashComponent::DashCD()
{
	m_bCanDash = true;
}

void UDashComponent::DashSlowMoReset()
{
	m_bIsSlowMo = false;
	m_bIsPerfectDodge = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
	m_ownerRef->CustomTimeDilation = 1;
}

void UDashComponent::PerfectDodge()
{
	m_bIsPerfectDodge = true;
}

void UDashComponent::SetCanPerfectDodge()
{
	m_bCanPerfectDodge = true;
}

void UDashComponent::ImpulseEnnemy()
{
	FVector Start = m_ownerRef->GetActorLocation();
	const float CapsuleRadius = m_ownerRef->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2;
	const float CapsuleHalfHeight = m_ownerRef->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2;
	FCollisionShape SphereShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(m_ownerRef);
	GetWorld()->SweepMultiByObjectType(OutHits, Start, Start, FQuat::Identity, UEngineTypes::ConvertToTraceType(ECC_Visibility), SphereShape, QueryParams);

	DrawDebugCapsule(GetWorld(), m_ownerRef->GetActorLocation(), CapsuleHalfHeight, CapsuleRadius, m_ownerRef->GetActorRotation().Quaternion(), FColor::Red);
	
	for(auto It = OutHits.CreateIterator(); It; It++)
	{
		ACharacter* CharacterHited = Cast<ACharacter>((*It).GetActor());
		if(CharacterHited)
		{
			FVector Impulse = (CharacterHited->GetActorLocation() - m_ownerRef->GetActorLocation()).GetSafeNormal();
			Impulse.Z = m_impulseZ;
			CharacterHited->GetCharacterMovement()->AddImpulse(Impulse * m_impulse, true);
		}
	}
}

