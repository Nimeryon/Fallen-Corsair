// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeTargeting.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLENCORSAIR_API UMeleeTargeting : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeleeTargeting();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float ChampVision = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float SpeedMoveToActor = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float TargetMaxDistanceFromOwner = 350;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float DistanceSecurity = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float SpeedRotation = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	bool Debug = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = Properties)
	class AActor* GetTarget();

	UFUNCTION(BlueprintCallable, Category = Properties)
	bool IsMeleeTargeting();

	void MoveToActorTarget(float DeltaTime);
	void OrientOwnerToTarget(float DeltaTime);

	class AActor *ActorTarget;

	bool MovingToTargetEnded = false;
	bool RotationToTargetEnded = false;
	bool TargetReached = false;
		
private:

	// Functions
	TArray<class ACharacter*> GetAllCharactersTargetable();
	TArray<class AActor*> GetAllTargetagbleOnScreenAndOnOwnerVision();
	class AActor* GetActorDistanceLowestFromOwner(TArray<class AActor*> Actors);
	class AActor* GetActorMostOnOwnerMiddleVision(TArray<class AActor*> Actors);

	FVector GetActorWorldScreenLocation(class AActor &Actor);
	bool ActorIsOnScreen(class AActor &Actor, float Tolerance = 0);
	bool IsOnOwnerVision(class AActor &Actor);
	float GetLocationVisionFromOwner(class AActor &Actor);

	float ConvDegreeTo360(float Degree);
	bool TurnOnLeft(float CurrentDegree, float DegreeToReach, float fMin, float fMax);
	bool IsMovingToActorTarget = false;

};
