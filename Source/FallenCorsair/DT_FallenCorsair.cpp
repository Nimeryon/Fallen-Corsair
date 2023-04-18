// Fill out your copyright notice in the Description page of Project Settings.


#include "DT_FallenCorsair.h"

void UDT_FallenCorsair::SetDamageType(EDamageType DamageType) { m_type = DamageType; }

EDamageType UDT_FallenCorsair::GetDamageType() const { return m_type; }

