// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FallenCorsairCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShoot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDodge);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPerfectDodge);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSoulPicked);

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
		class UInputAction* MeleeSoftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MeleeHeavyAction;

	/** Melee Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MeleeCancelAction;

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

	UPROPERTY(EditDefaultsOnly)
	class UDashComponent* dashComp;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Called for melee input
	void MeleeSoftStarted(const FInputActionValue& Value);

	void MeleeHeavyStarted(const FInputActionValue& Value);
	void MeleeHeavyCompleted(const FInputActionValue& Value);
	void MeleeHeavyTriggered(const FInputActionValue& Value);

	void MeleeSetRotation(const FInputActionValue& Value);
	void MeleeResetRotation(const FInputActionValue& Value);

	void ShootCompleted(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerSpawn OnPlayerSpawn;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void Aim(const FInputActionValue& bIsZoom);

	UFUNCTION()
	void Charge(const FInputActionValue& value);

	UFUNCTION()
	TArray<AActor*> SetIgnoreCharacterActors();

	UFUNCTION()
	void SetCanBeDamage(bool bCanBeDammage);

	UPROPERTY()
	FOnShoot OnShoot;

	UPROPERTY()
	FOnAim OnAim;
	
	bool IsStunned = false;

	UPROPERTY()
	FOnDodge OnDodge;

	UPROPERTY()
	FOnPerfectDodge OnPerfectDodge;

	UPROPERTY()
	FOnSoulPicked OnSoulPicked;

#pragma region Input Availability

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDashAvailable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAimAvailable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsShootAvailable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSoftMeleeAvailable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHeavyMeleeAvailable = true;

#pragma endregion 

#pragma region Melee


	// Chrono for melee input
	float Melee_TriggeredSeconds = 0;
	bool Melee_IsTrigerred = false;
#pragma endregion 

#pragma region Camera Variables
	UPROPERTY(BlueprintReadWrite)
	bool m_bIsFocus = false;

	UPROPERTY()
	bool m_bIsCharge = false;

	UPROPERTY()
	float m_alpha = 0.f;

	UPROPERTY()
	float m_alphaCharge = 0.f;
	
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

	UPROPERTY(EditAnywhere, Category = "Camera Option|Field of view", meta = (displayName = "Field of view enCharge"), meta = (ClampMin = 10.f, UIMin = 10.f, ClampMax = 170.f, UIMax = 170.f))
	float m_fieldOfView_C = 120.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Distance", meta = (displayName = "Distance de la camera standard"), meta = (ClampMin = 100.f, UIMin = 100.f, ClampMax = 2000.f, UIMax = 2000.f))
	float m_distanceFromPlayer_S = 400.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Distance", meta = (displayName = "Distance de la camera Aim"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 2000.f, UIMax = 2000.f))
	float m_distanceFromPlayer_A = 40.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Distance", meta = (displayName = "Distance de la camera en Charge"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 2000.f, UIMax = 2000.f))
	float m_distanceFromPlayer_C = 40.f;

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

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Standard", meta = (displayName = "Camera bottom limit standard"), meta = (ClampMin = -90, UIMin = -90, ClampMax = 90, UIMax = 90))
	float m_pitchMin_S = -70.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Standard", meta = (displayName = "Camera top limit standard"), meta = (ClampMin = -90, UIMin = -90, ClampMax = 90, UIMax = 90))
	float m_pitchMax_S = 70.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Aim", meta = (displayName = "Camera bottom limit Aim"), meta = (ClampMin = -90, UIMin = -90, ClampMax = 90, UIMax = 90))
	float m_pitchMin_A = -45.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option|Clamp|Aim", meta = (displayName = "Camera top limit Aim"), meta = (ClampMin = -90, UIMin = -90, ClampMax = 90, UIMax = 90))
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

	UPROPERTY(EditAnywhere, Category = "Camera Option|Default", meta = (displayName = "Camera Shake"))
	TSubclassOf<UCameraShakeBase> m_cameraShake;
	
	UPROPERTY(EditAnywhere, Category = "Camera Option|Opacity", meta = (displayName = "Opacity Distance Max"))
    float m_opacityDistanceMax = 160;
    
    UPROPERTY(EditAnywhere, Category = "Camera Option|Opacity", meta = (displayName = "Opacity Distance Min"))
	float m_opacityDistanceMin = 80;
	
	UPROPERTY()
	UCameraShakeBase* m_currentCameraShake;

#pragma endregion

#pragma region Health Variables

	UPROPERTY()
	bool m_bCanBeDammage = true;

	UPROPERTY()
	bool m_bIsLowHP = false;

	UPROPERTY()
	float m_currentHealth = 50.f;

	UPROPERTY()
	float m_alphaRecover = 1.f;

	UPROPERTY()
	float m_alphaDerecover = 0;

	UPROPERTY()
	bool m_bIsHealing = false;
	
	UPROPERTY(EditAnywhere, Category = "Vie", meta = (displayName = "Vie du joueur"), meta = (ClampMin = 1, UIMin = 1, ClampMax = 1000, UIMax = 1000))
	float m_maxHealth = 50.f;

	/**
	 * Define The Percentage Of Health That The Player Will Recover Each Second
	 */
	UPROPERTY(EditAnywhere, Category = "Vie", meta = (displayName = "Pourcentage de récupétation"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 100, UIMax = 100))
	float m_recovery = 50.f;

	/**
	 * Define when the player is in low HP
	 */
	UPROPERTY(EditAnywhere, Category = "Vie", meta = (displayName = "Vie du joueur Low"), meta = (ClampMin = 0, UIMin = 0, ClampMax = 100, UIMax = 100))
	float m_lowHP = 20.f;

	UPROPERTY(EditAnywhere, Category = "Vie", meta = (displayName = "Fade Speed"), meta = (ClampMin = 0.1, UIMin = 0.1, ClampMax = 100, UIMax = 100))
	float m_changeSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category = "Vie", meta = (displayName = "MPC"))
	UMaterialParameterCollection* m_collection;

#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Walk", meta = (displayName = "Normal Walk"))
	float m_walkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Walk", meta = (displayName = "Aim Walk"))
	float m_aimWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Walk", meta = (displayName = "Charge Walk"))
	float m_chargeWalkSpeed = 600.f;
	
	
};

