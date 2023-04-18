// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "InputActionValue.h"
#include "Melee.generated.h"


UENUM()
enum class EMeleeCollisionShape
{
	Box,
	Sphere,
	Capsule
};


USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float PropulsionForceOwner = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	FVector PropulsionDirectionOwner = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float PropulsionForceEnnemie = 0;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	FVector PropulsionDirectionEnnemie = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool PropulsionEnnemieDirectionFromOwner = false;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool PropulsionEnnemieDirectionFromOwnerNormalize2D = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape")
	EMeleeCollisionShape CollisionShape = EMeleeCollisionShape::Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape")
	FVector CollisionShapeOffset = FVector(0, 0, 0);	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape")
	FRotator CollisionShapeRotation = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShapeBox")
	FVector BoxSize = FVector(100, 100, 100);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShapeSphere")
	float SphereRadius = 100;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShapeCapsule")
	float CapsuleHalfHeight = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShapeCapsule")
	float CapsuleRadius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	bool CollisionWithSockets = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	FName SocketStart = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	FName SocketEnd = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShapeSphere")
	float SocketSphereRadius = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dammage")
	float Dammage = 0;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float MinDammage = 0;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float MinDistance = 0;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float MaxDistance = 0;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	UAnimMontage* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	class USoundBase* PlayerVoiceSound;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	class USoundBase* AttackSound;

};

USTRUCT(BlueprintType)
struct FMelees
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAttackData> Soft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAttackData> Heavy;
};

UENUM()
enum class EAttackType
{
	Soft,
	Heavy,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeleguateMelee);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FALLENCORSAIR_API UMelee : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMelee();

	UFUNCTION()
	void OnMyDelegateTriggered();

	UPROPERTY(BlueprintAssignable, Category = "MyDelegate")
	FDeleguateMelee DeleguateMelee;

	// Vars
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	FMelees Melees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	UAnimMontage* AnimWhileChargingMeleeHeavy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	bool CanAirAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float delayInputDepthMeleeHeavy = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	bool Debug = false;

	// Functions
	//UFUNCTION(BlueprintCallable, Category = Properties)
	virtual void PerformAttack();
	virtual void PlayAnimationChargingMeleeHeavy();
	virtual void StopAnimationChargingMeleeHeavy();
	virtual void SetTypeAttack(EAttackType at);
	virtual void StartAttack(bool start);
	virtual void UpdateTypeAttack(float& eslapsedSeconds);
	virtual void SetReleased(bool released);
	virtual void SetOwnerModeAttack(bool ModeAttack);
	virtual void CancelAttack();
	virtual bool MeleeIsValid();
	virtual bool AttackIsStarted();
	virtual bool IsReleased() const;
	virtual void CalculRotation(FVector _rot);
	virtual void ResetRotation();
	virtual void ResetCombo();
	virtual bool IsFirstCombo();
	virtual bool IsLastCombo();


	UFUNCTION(BlueprintCallable, Category = Properties)
	void TriggerHitWithSockets();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	// Reaction to elements hited
	virtual void DammageOnHits(TArray<FHitResult> OutHits);

private:

	// Functions
	void TriggerHitWithCollisionShape();

	// Character
	virtual void SetRotation();
	virtual void FreezeRotation(bool freeze);
	virtual void EnableWalk(bool enable);

	// Attack
	virtual void AttackSequence();
	virtual void PropulseOwner();
	virtual void ResetVelocity();
	virtual void IncrementCurrentAttack();
	virtual FAttackData& GetCurrentMelee();


	// Vars

	class ACharacter* OwnerCharacter;

	// To avoid multiple hit while TriggerWithSokect
	class TArray<ACharacter*> CharactersHited;

	// To disabled Character walk
	float MaxWalkSpeed;
	float MinWalkSpeed;

	EAttackType attackType = EAttackType::Soft;

	int indexCurrentAttack = 0;

	bool bCanAttack = true;
	bool bCanExecuteNextAttack = false;
	bool bExecuteNextAttack = false;
	bool bAttackStarted = false;
	bool bInputReleased = false;
	bool bIsDeleguate = false;
	FRotator RotatorWhileAttackStarted;

};