// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "CharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyFirstGameCharacter.generated.h"


UCLASS(config=Game)
class AMyFirstGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMyFirstGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Getter and setter for health components
	float get_health() const;
	float get_max_health() const;
	void set_health(float const new_health);

	virtual void attack_start();
	virtual void attack_end();

	//UFUNCTION(BlueprintPure, Category = "Power")
	//float GetRocksToWin() const;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	//float RocksToWin;

	UFUNCTION(BlueprintPure, Category = "Rock")
	float GetInitialRocks();

	UFUNCTION(BlueprintPure, Category = "Rock")
	float GetCurrentRocks();

	UFUNCTION(BlueprintCallable, Category = "Rock")
	void UpdateRocks();

	void Tick(float DeltaSeconds) override;

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Particle", meta = (AllowProtectedAccess = "true"))
	bool GetIsGrabbing();

	void SetIsGrabbing(bool IsGrabbing);
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* fist_collision_box;

	void SwitchCamera();

	bool bIsGrabbing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocks")
	float InitialRocks;
	
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* montage;

	class UWidgetComponent* widget_component;
	float const max_health = 30.0f;
	float health;

	// Current Rocks collected
	UPROPERTY(VisibleAnywhere, Category= "Rocks")
	float CharacterRocks;
	
	void Switch();

	void on_attack();

	bool bFirstPersonCamera;

	class UAIPerceptionStimuliSourceComponent* stimulus;

	void setup_stimulus();

	UFUNCTION()
	void on_attack_overlap_begin(
		UPrimitiveComponent* const overlapped_component,
		AActor* const other_actor,
		UPrimitiveComponent* other_component,
		int const other_body_index,
		bool const from_sweep,
		FHitResult const& sweep_result);

	UFUNCTION()
	void on_attack_overlap_end(
		UPrimitiveComponent* const overlapped_component,
		AActor* const other_actor,
		UPrimitiveComponent* other_component,
		int const other_body_index);
};

