// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Melee.generated.h"

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float PropulsionForceOwner = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float PropulsionForceForwardEnnemie = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float PropulsionForceUpEnnemie = 0;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	FVector BoxOffset = FVector(0, 0, 0);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	FVector BoxSize = FVector(100, 100, 100);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float RecoveryTime = 1; // Seconde
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	UAnimMontage *Anim;
	
};

UENUM()
enum class EAttackType
{
	Soft,
	Heavy,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLENCORSAIR_API UMelee : public UActorComponent
{
	GENERATED_BODY()

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

public:	
	// Sets default values for this component's properties
	UMelee();

	// Vars
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	TArray<FAttackData> MeleesSoft;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	bool Debug = false;


	// Functions
	UFUNCTION(BlueprintCallable, Category = Properties)
	virtual void PerformAttack();
	
	virtual void SetTypeAttack(EAttackType at);
	virtual void StartAttack(bool start);
	virtual void UpdateTypeAttack(float eslapsedSeconds);
	virtual bool AttackIsStarted();
	virtual void SetReleased(bool released);
	virtual void CancelAttack();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	
	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	virtual void TriggerHit();
	
private:

	// Functions

	// Character
	virtual void SetRotation();
	virtual void CalculRotation(FVector _rot);
	virtual void FreezeRotation(bool freeze);
	virtual void EnableWalk(bool enable);

	// Attack
	virtual void AttackSequence();
	virtual void PropulseOwner();
	virtual void ResetVelocity();
	virtual void ResetCombo();
	virtual void ResetState();
	virtual bool IsLastCombo();
	virtual FAttackData &GetCurrentMelee();


	// Vars
	ACharacter* ownerCharacter;
	float maxWalkSpeed;

	EAttackType attackType = EAttackType::Soft;

	int indexCurrentAttack = 0; 
	float delayInputDepth = 1;

	bool bCanAttack = true;
	bool bCanExecuteNextAttack = false;
	bool bExecuteNextAttack = false;
	bool bAttackStarted = false;
	bool bInputReleased = false;
	bool bIsDeleguate = false;


	FVector rotation = FVector(0, 0, 0);

	UAnimInstance* AnimDeleguate;
};
