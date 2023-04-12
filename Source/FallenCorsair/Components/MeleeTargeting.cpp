// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeTargeting.h"
#include "Targetable.h"
#include "Engine/Internal/Kismet/BlueprintTypeConversions.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UMeleeTargeting::UMeleeTargeting()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeleeTargeting::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMeleeTargeting::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!ActorTarget)
		return;
	
	OrientOwnerToTarget(DeltaTime);
	MoveToActorTarget(DeltaTime);

	if (MovingToTargetEnded && RotationToTargetEnded)
	{
		TargetReached = true;
		IsMovingToActorTarget = false;
		ActorTarget = nullptr;
	}
}

// Public

AActor* UMeleeTargeting::GetTarget()
{
	TArray<AActor*> Actors = GetAllTargetagbleOnScreenAndOnOwnerVision();

	AActor* Actor = GetActorDistanceLowestFromOwner(Actors);

	ActorTarget = Actor;

	if (ActorTarget)
	{
		RotationToTargetEnded = false;
		MovingToTargetEnded = false;
		IsMovingToActorTarget = true;
	}

	return Actor;
}

// Private

TArray<ACharacter*> UMeleeTargeting::GetAllCharactersTargetable()
{
	// Get all instances of MyActorClass
	TArray< AActor*> FoundActors;
	TSubclassOf<ACharacter> ClassToFind;
	TArray<ACharacter*> CharactersTargetable;

	ClassToFind = ACharacter::StaticClass();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);
	
	// Do something with the found actors
	for (AActor *FoundActor : FoundActors)
	{
		if (FoundActor->GetComponentByClass(UTargetable::StaticClass()))
		{
			ACharacter *Character = Cast<ACharacter>(FoundActor);
			if (Character)
				CharactersTargetable.Add(Character);
		}
	}
	
	return CharactersTargetable;
}

TArray<AActor*> UMeleeTargeting::GetAllTargetagbleOnScreenAndOnOwnerVision()
{
	TArray<ACharacter*> CharactersTargetable = GetAllCharactersTargetable();
	TArray<AActor*> Characters;
	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, UKismetStringLibrary::Conv_IntToString(CharactersTargetable.Num()));

	for (ACharacter* Character : CharactersTargetable)
	{
		if (ActorIsOnScreen(*Character) && IsOnOwnerVision(*Character))
		{
			float Distance = UKismetMathLibrary::Vector_Distance(Character->GetActorLocation(), GetOwner()->GetActorLocation());
			if (Distance <= TargetMaxDistanceFromOwner)
			{
				Characters.Add(Character);
			}
		}
	}

	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, UKismetStringLibrary::Conv_IntToString(Characters.Num()));


	return Characters;
}

AActor* UMeleeTargeting::GetActorDistanceLowestFromOwner(TArray<AActor*> Actors)
{
	AActor *Actor = nullptr;
	
	if (!Actors.Num())
		return Actor;

	float LowestDistance;

	float Distance = UKismetMathLibrary::Vector_Distance(Actors[0]->GetActorLocation(), GetOwner()->GetActorLocation());
	LowestDistance = Distance;
	Actor = Actors[0];

	for (AActor *Actorr : Actors)
	{
		Distance = UKismetMathLibrary::Vector_Distance(Actorr->GetActorLocation(), GetOwner()->GetActorLocation());
		if (Distance < LowestDistance)
		{
			LowestDistance = Distance;
			Actor = Actorr;
		}
	}

	return Actor;
}

FVector UMeleeTargeting::GetActorWorldScreenLocation(AActor &Actor)
{
	FVector WorldScreenLocationActor;
	APlayerController* Controller  = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Get the player controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	// Get the viewport dimensions
	int32 ViewportWidth, ViewportHeight;
	PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);

	// Convert the world location to screen location
	FVector2D ScreenLocation;
	FVector WorldLocation = Actor.GetActorLocation();
	PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation);

	// Convert the screen location to pixel coordinates
	int32 ScreenX = FMath::RoundToInt(ScreenLocation.X * ViewportWidth);
	int32 ScreenY = FMath::RoundToInt(ScreenLocation.Y * ViewportHeight);

	return UKismetMathLibrary::Conv_Vector2DToVector(ScreenLocation);
}

bool UMeleeTargeting::ActorIsOnScreen(AActor &Actor, float Tolerance)
{
	FVector ActorWSL = GetActorWorldScreenLocation(Actor);
	
	// Get the player controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	// Get the viewport dimensions
	int32 ViewportWidth, ViewportHeight;
	PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);
	
	if (
		((ActorWSL.X + Tolerance) > 0 && (ActorWSL.X < ViewportWidth)) && 
		((ActorWSL.Y - Tolerance) > 0 && ((ActorWSL.Y - Tolerance) < ViewportHeight))
	)
	{
		return true;
	}

	return false;
}

bool UMeleeTargeting::IsOnOwnerVision(AActor &Actor)
{
	FVector DirOwnerToActor = Actor.GetActorLocation() - GetOwner()->GetActorLocation();
	DirOwnerToActor.Normalize();
	float dot = UKismetMathLibrary::Dot_VectorVector(DirOwnerToActor, GetOwner()->GetActorForwardVector());
	return acos(dot) <= ChampVision;
}

bool UMeleeTargeting::TurnOnLeft(float CurrentDegree, float DegreeToReach, float fMin, float fMax)
{
	if (CurrentDegree + fMin == DegreeToReach)
		return true;
	else
	{
		if (CurrentDegree - fMin == DegreeToReach)
			return false;
		else
		{
			if (CurrentDegree - fMax < 0)
				return false;
			
			return true;
		}
	}
}

void UMeleeTargeting::MoveToActorTarget(float DeltaTime)
{	
	if (!ActorTarget)
		return;

	float Distance = UKismetMathLibrary::Vector_Distance(ActorTarget->GetActorLocation(), GetOwner()->GetActorLocation());

	if (Distance <= DistanceSecurity)
	{
		MovingToTargetEnded = true;
	}
	else
	{
		FVector Direction = ActorTarget->GetActorLocation() - GetOwner()->GetActorLocation();
		FVector2D Direction2D = FVector2D(Direction.X, Direction.Y);
		Direction2D.Normalize();
		Direction = FVector(Direction2D.X, Direction2D.Y, 0);
		FVector NewLocation = Direction * SpeedMoveToActor * DeltaTime;
		NewLocation += GetOwner()->GetActorLocation();
		GetOwner()->SetActorLocation(NewLocation);
	}
}

void UMeleeTargeting::OrientOwnerToTarget(float DeltaTime)
{
	if (!ActorTarget)
		return;

	FVector Direction;
	FVector2D Direction2D;
	FRotator RotatorToTarget;
	float CurrentDegree, DegreeToReach;

	Direction = ActorTarget->GetActorLocation() - GetOwner()->GetActorLocation();
	Direction2D = FVector2D(Direction.X, Direction.Y);
	Direction2D.Normalize();
	Direction = FVector(Direction2D.X, Direction2D.Y, 0);
	
	RotatorToTarget = UKismetMathLibrary::Conv_VectorToRotator(Direction);
	
	CurrentDegree = GetOwner()->GetActorRotation().Yaw;
	DegreeToReach = RotatorToTarget.Yaw;

	if (abs(CurrentDegree - DegreeToReach) <= (SpeedRotation * DeltaTime))
	{
		GetOwner()->SetActorRotation(RotatorToTarget);
		RotationToTargetEnded = true;
	}
	else
	{
		CurrentDegree = ConvDegreeTo360(CurrentDegree);
		DegreeToReach = ConvDegreeTo360(DegreeToReach);

		float QuantityDegreeToRush = CurrentDegree - DegreeToReach;
		QuantityDegreeToRush = abs(QuantityDegreeToRush);
		
		float DegreeMin = std::min(QuantityDegreeToRush,  360 - QuantityDegreeToRush);
		float DegreeMax = std::max(QuantityDegreeToRush,  360 - QuantityDegreeToRush);

		FRotator AddRotator;
		AddRotator.Yaw = SpeedRotation * DeltaTime;

		if (!TurnOnLeft(CurrentDegree, DegreeToReach, DegreeMin, DegreeMax))
			AddRotator.Yaw *= -1;
		
		GetOwner()->AddActorLocalRotation(AddRotator);
	}
}

float UMeleeTargeting::ConvDegreeTo360(float Degree)
{
	if (Degree < 0)
	{
		return abs(360 + Degree);
	}

	return Degree;
}



