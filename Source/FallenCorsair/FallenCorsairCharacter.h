// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FallenCorsairCharacter.generated.h"


UCLASS(config=Game)
class AFallenCorsairCharacter : public ACharacter
{
	GENERATED_BODY()

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction;

	

public:
	AFallenCorsairCharacter();
	
	UPROPERTY(EditDefaultsOnly)
	class UBarrel* barrelComp;

	UPROPERTY(EditDefaultsOnly)
	class UGun* gunComp;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	bool m_bIsFocus = false;

	UPROPERTY()
	float m_alpha = 0.f;
	
	UPROPERTY()
	float m_direction = 0.f;

	UPROPERTY()
	APlayerCameraManager* CameraManager;
	
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
	float m_distanceFromPlayer_A = 0;

	/**
	 * X IS Horizontal And Y Is Vertical
	 */
	UPROPERTY(EditAnywhere, Category = "Camera Option|Offset", meta = (displayName = "Camera Offset standard"))
	FVector2D m_CameraOffset_S = FVector2D(0,0);

	/**
	 * X IS Horizontal And Y Is Vertical
	 */
	UPROPERTY(EditAnywhere, Category = "Camera Option|Offset", meta = (displayName = "Camera Offset Aim"))
	FVector2D m_CameraOffset_A = FVector2D(0,0);

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Standard", meta = (displayName = "Camera bottom limit standard"), meta = (ClampMin = -90, UIMin = -90, ClampMax = -1, UIMax = -1))
	float m_pitchMin_S = -90.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Standard", meta = (displayName = "Camera top limit standard"), meta = (ClampMin = 1, UIMin = 1, ClampMax = 90, UIMax = 90))
	float m_pitchMax_S = 90.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Aim", meta = (displayName = "Camera bottom limit Aim"), meta = (ClampMin = -90, UIMin = -90, ClampMax = -1, UIMax = -1))
	float m_pitchMin_A = -90.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Aim", meta = (displayName = "Camera top limit Aim"), meta = (ClampMin = 1, UIMin = 1, ClampMax = 90, UIMax = 90))
	float m_pitchMax_A = 90.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Mouse", meta = (displayName = "Sensibilité de la souris"), meta = (ClampMin = 0.2, UIMin = 0.2, ClampMax = 3, UIMax = 3))
	float m_mouseSensitivity_S = 1.f;

	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void Shoot();

	UFUNCTION()
	void Aim(const FInputActionValue& bIsZoom);


};

