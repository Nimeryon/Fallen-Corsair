// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FallenCorsairCharacter.generated.h"

UENUM()
enum class ECustomMovementMode
{
	Default,
	Dash,
};

// Event dispatcher OnPlayerSpawn
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerSpawn);

UCLASS(config=Game)
class AFallenCorsairCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	class UBrutosMovementComponent* BrutosMovementComponent;
	
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

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


	// Shoot Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	// Aim Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction;

	// Charge Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction*	ChargeAction;

public:
	AFallenCorsairCharacter(const FObjectInitializer& ObjectInitializer);

	/** Melee Component */
	UPROPERTY(EditAnywhere, Category = Melee, meta = (AllowPrivateAccess = "true"))
		class UMelee* MeleeComponent;

	UPROPERTY(EditAnywhere, Category = MeleeTargeting, meta = (AllowPrivateAccess = "true"))
		class UMeleeTargeting* MeleeTargetingComponent;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBarrel* barrelComp;

	UPROPERTY(EditDefaultsOnly)
	class UGun* gunComp;

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
	UPROPERTY(BlueprintAssignable)
	FOnPlayerSpawn OnPlayerSpawn;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void Shoot();

	UFUNCTION()
	void Aim(const FInputActionValue& bIsZoom);

	UFUNCTION()
	void Charge(const FInputActionValue& value);

private:

	// Chrono for melee input
	float Melee_TriggeredSeconds = 0;
	bool Melee_IsTrigerred = false;

#pragma region Camera Variables
	UPROPERTY()
	bool m_bIsFocus = false;

	UPROPERTY()
	float m_alpha = 0.f;
	
	UPROPERTY()
	float m_direction = 0.f;

	UPROPERTY()
	APlayerCameraManager* m_cameraManager;
	
	UPROPERTY(EditAnywhere, Category = "Camera Option|Default", meta = (displayName = "Camera Pitch"), meta = (ClampMin = -90, UIMin = -90, ClampMax = 90, UIMax = 90))
	float m_pitchAngle = 0;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Default", meta = (displayName = "Camera lag"), meta = (ClampMin = 1, UIMin = 1))
	float m_cameraLag = 15.f;
	
	UPROPERTY(EditAnywhere, Category = "Camera Option|Transition", meta = (displayName = "Transition de visée en zoom"), meta = (ClampMin = 0.01f, UIMin = 0.01f, ClampMax = 10, UIMax = 10))
	float m_transitionSpeedZoom = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Transition", meta = (displayName = "Transition de visée en dezoom"), meta = (ClampMin = 0.01f, UIMin = 0.01f, ClampMax = 10, UIMax = 10))
	float m_transitionSpeedDezoom = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Field of view", meta = (displayName = "Field of view standard"), meta = (ClampMin = 30.f, UIMin = 30.f, ClampMax = 170.f, UIMax = 170.f))
	float m_fieldOfView_S = 90.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Field of view", meta = (displayName = "Field of view Aim"), meta = (ClampMin = 10.f, UIMin = 10.f, ClampMax = 170.f, UIMax = 170.f))
	float m_fieldOfView_A = 90.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Distance", meta = (displayName = "Distance de la camera standard"), meta = (ClampMin = 100.f, UIMin = 100.f, ClampMax = 2000.f, UIMax = 2000.f))
	float m_distanceFromPlayer_S = 400.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Distance", meta = (displayName = "Distance de la camera Aim"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 2000.f, UIMax = 2000.f))
	float m_distanceFromPlayer_A = 40.f;

	/**
	 * X IS Horizontal And Y Is Vertical
	 */
	UPROPERTY(EditAnywhere, Category = "Camera Option|Offset", meta = (displayName = "Camera Offset standard"))
	FVector2D m_CameraOffset_S = FVector2D(0,0);

	/**
	 * X IS Horizontal And Y Is Vertical
	 */
	UPROPERTY(EditAnywhere, Category = "Camera Option|Offset", meta = (displayName = "Camera Offset Aim"))
	FVector2D m_CameraOffset_A = FVector2D(30.f,70.f);

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Standard", meta = (displayName = "Camera bottom limit standard"), meta = (ClampMin = -90, UIMin = -90, ClampMax = -1, UIMax = -1))
	float m_pitchMin_S = -70.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Standard", meta = (displayName = "Camera top limit standard"), meta = (ClampMin = 1, UIMin = 1, ClampMax = 90, UIMax = 90))
	float m_pitchMax_S = 70.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Aim", meta = (displayName = "Camera bottom limit Aim"), meta = (ClampMin = -90, UIMin = -90, ClampMax = -1, UIMax = -1))
	float m_pitchMin_A = -45.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Aim", meta = (displayName = "Camera top limit Aim"), meta = (ClampMin = 1, UIMin = 1, ClampMax = 90, UIMax = 90))
	float m_pitchMax_A = 45.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Mouse|Standard", meta = (displayName = "Sensibilité de la souris vertical standard"), meta = (ClampMin = 0.1, UIMin = 0.1, ClampMax = 5, UIMax = 5))
	float m_mousePitchSensitivity_S = 1.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Mouse|Standard", meta = (displayName = "Sensibilité de la souris horizontal standard"), meta = (ClampMin = 0.1, UIMin = 0.1, ClampMax = 5, UIMax = 5))
	float m_mouseYawSensitivity_S = 1.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Mouse|Aim", meta = (displayName = "Sensibilité de la souris vertical Aim"), meta = (ClampMin = 0.1, UIMin = 0.1, ClampMax = 5, UIMax = 5))
	float m_mousePitchSensitivity_A = 1.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Mouse|Aim", meta = (displayName = "Sensibilité de la souris horizontal Aim"), meta = (ClampMin = 0.1, UIMin = 0.1, ClampMax = 5, UIMax = 5))
	float m_mouseYawSensitivity_A = 1.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Default", meta = (displayName = "Camera Curve"))
	UCurveFloat* m_cameraCurve;

#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Vie", meta = (displayName = "Vie du joueur"), meta = (ClampMin = 1, UIMin = 1, ClampMax = 1000, UIMax = 1000))
	float m_health = 50.f;

	
};

