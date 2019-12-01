// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "GravityGunCameraShake.generated.h"

/**
 *  Class for simpler implementation of camera shake, using only rotational oscillation
 */
UCLASS()
class GRAVITYGUN_API UGravityGunCameraShake : public UCameraShake
{
	GENERATED_BODY()
	
public:
	//Default constructor
	UGravityGunCameraShake();

	//Inherited overridden functions
	virtual void UpdateAndApplyCameraShake(float DeltaTime, float Alpha, FMinimalViewInfo& InOutPOV) override;
	virtual void PlayShake(class APlayerCameraManager* Camera, float Scale, ECameraAnimPlaySpace::Type InPlaySpace, FRotator UserPlaySpaceRot = FRotator::ZeroRotator) override;

protected:

	//Used to control the rotational pitch, the amplitude and the frequency
	UPROPERTY(VisibleAnywhere, Category = "Rotation Oscillation")
	FFOscillator RotPitch;

	//Used to control the rotational yaw, the amplitude and the frequency
	UPROPERTY(VisibleAnywhere, Category = "Rotation Oscillation")
	FFOscillator RotYaw;
	
public:

	//Set the rotational pitch
	UFUNCTION(BlueprintCallable, Category = "Rotation Oscillation")
	void SetRotPitch(float Amplitude, float Frequency);

	//Set the rotational yaw
	UFUNCTION(BlueprintCallable, Category = "Rotation Oscillation")
	void SetRotYaw(float Amplitude, float Frequency);

	//Get the rotational pitch
	UFUNCTION(BlueprintCallable, Category = "Rotation Oscillation")
	FORCEINLINE	FFOscillator GetRotPitch() const { return RotPitch; }

	//Get the rotational yaw
	UFUNCTION(BlueprintCallable, Category = "Rotation Oscillation")
	FORCEINLINE	FFOscillator GetRotYaw() const { return RotYaw; }

	
	
};
