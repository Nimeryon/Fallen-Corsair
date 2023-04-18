// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DT_FallenCorsair.generated.h"

UENUM()
enum EDamageType
{
	MeleeSoft,
	MeleeHeavy,
	Distance,
	Explosion,
	Default
};

UCLASS()
class FALLENCORSAIR_API UDT_FallenCorsair : public UDamageType
{
	GENERATED_BODY()

public:
	void SetDamageType(EDamageType DamageType);
	
	EDamageType GetDamageType() const;
	
	UPROPERTY()
	TEnumAsByte<EDamageType> m_type = EDamageType::Default;
};
