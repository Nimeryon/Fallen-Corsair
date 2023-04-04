// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"

struct MaStructure {

	float PropulsionForceOwner = 0;
	float PropulsionForceForwardEnnemie = 0;
	float PropulsionForceUpEnnemie = 0;
	float RecoveryTime = 0;
	float BoxOffsetX = 0;

	FVector BoxSize = FVector(100, 100, 100);
	UAnimMontage Anim
}