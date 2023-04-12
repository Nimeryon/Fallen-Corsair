// Copyright Epic Games, Inc. All Rights Reserved.

#include "FallenCorsairGameMode.h"
#include "FallenCorsairCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Waves/WaveTracker.h"

AFallenCorsairGameMode::AFallenCorsairGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	CreateDefaultSubobject<UWaveTracker>(TEXT("WaveTracker"));
}
