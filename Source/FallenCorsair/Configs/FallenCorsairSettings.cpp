// Fill out your copyright notice in the Description page of Project Settings.


#include "FallenCorsairSettings.h"

float UConfigUtils::GetMaxAICount(){
	const UFallenCorsairSettings* Settings = GetDefault<UFallenCorsairSettings>();
	return Settings->MaxAICount;
}
