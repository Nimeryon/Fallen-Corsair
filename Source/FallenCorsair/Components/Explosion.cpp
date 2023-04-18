// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "NiagaraSystem.h"

// Sets default values for this component's properties
UExplosion::UExplosion()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Public

void UExplosion::PerformExplosion(const UWorld* InWorld, AActor *Self, float Dammage, FVector Origin, float SphereRadius, float PropulsionForce, float RotationAngleDegrees, UNiagaraSystem* NS_Explosion, bool Debug)
{
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereRadius);

	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Self);

	InWorld->SweepMultiByObjectType(OutHits, Origin, Origin, FQuat::Identity, UEngineTypes::ConvertToTraceType(ECC_Visibility), SphereShape, QueryParams);

	for (auto It = OutHits.CreateIterator(); It; It++)
	{
		ACharacter* CharacterHited = Cast<ACharacter>((*It).GetActor());

		if (CharacterHited)
		{
			// Propulse
			FVector Dir = CharacterHited->GetActorLocation() - Self->GetActorLocation();
			FVector2D Dir2D = FVector2D(Dir.X, Dir.Y);
			Dir2D.Normalize();
			Dir = FVector(Dir2D.X, Dir2D.Y, 0);

			// Get the current forward direction
			FVector ForwardDirection = Dir;

			// Get the "up" direction (e.g. the world Z-axis)
			FVector UpDirection = FVector::UpVector;

			// Calculate the rotation axis from the cross product of the forward and up directions
			FVector RotationAxis = FVector::CrossProduct(ForwardDirection, UpDirection).GetSafeNormal();

			// Calculate the rotation quaternion from the rotation axis and the specified angle
			FQuat RotationQuat = FQuat(RotationAxis, FMath::DegreesToRadians(RotationAngleDegrees));

			// Rotate the forward direction using the rotation quaternion
			FVector RotatedDirection = RotationQuat.RotateVector(ForwardDirection);
			
			FVector Force = RotatedDirection * PropulsionForce;
			CharacterHited->GetCharacterMovement()->AddImpulse(Force, true);


			// Call Take dammage of character hited
			FDamageEvent EventDamage;
			CharacterHited->TakeDamage(Dammage, EventDamage, nullptr, Self);
		}
	}

	// Niagara Explosion
	if (NS_Explosion)
	{
		// Check if the Niagara FX asset was loaded successfully
		// Set up the spawn parameters
		FVector SpawnLocation = Self->GetActorLocation();
		FVector Scale = FVector(1, 1, 1);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		// Spawn the Niagara FX system at the specified location and rotation
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, NS_Explosion, SpawnLocation, SpawnRotation,  Scale, true);
	}

	if (Debug)
	{
		FColor Color = FColor::Red;
		if (OutHits.Num())
			Color = FColor::Green;
		DrawDebugSphere(InWorld, Origin, SphereRadius, 10, Color, false, 1, 0, 1);
	}
}


// Protected

// Called when the game starts
void UExplosion::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UExplosion::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


