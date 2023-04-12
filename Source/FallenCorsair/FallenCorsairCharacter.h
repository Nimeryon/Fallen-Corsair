// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FallenCorsairCharacter.generated.h"

UCLASS(config = Game)
class AFallenCorsairCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** Melee Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Melee, meta = (AllowPrivateAccess = "true"))
	class UMelee* MeleeComponent;

	/** Melee Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Melee, meta = (AllowPrivateAccess = "true"))
	class UMeleeTargeting* MeleeTargetingComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	/** Melee Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MeleeAction;

	/** Melee Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MeleeCancelAction;



public:
	AFallenCorsairCharacter();

	UPROPERTY(EditDefaultsOnly)
		class UBarrel* barrelComp;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Called for melee input
	void MeleeTriggered(const FInputActionValue& Value);
	void MeleeStarted(const FInputActionValue& Value);
	void MeleeCompleted(const FInputActionValue& Value);
	void MeleeSetRotation(const FInputActionValue& Value);
	void MeleeResetRotation(const FInputActionValue& Value);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:

	// Chrono for melee input
	float Melee_TriggeredSeconds = 0;
	bool Melee_IsTrigerred = false;
};

