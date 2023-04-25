// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_AlienPlant.h"

// Sets default values
AAC_AlienPlant::AAC_AlienPlant()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAC_AlienPlant::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAC_AlienPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAC_AlienPlant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

