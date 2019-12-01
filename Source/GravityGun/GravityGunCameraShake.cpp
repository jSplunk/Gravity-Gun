// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityGunCameraShake.h"

void UGravityGunCameraShake::SetRotPitch(float Amplitude, float Frequency)
{
	//Setting new values for the roational pitch of the oscillation
	RotPitch.Amplitude = Amplitude;
	RotPitch.Frequency = Frequency;

	RotOscillation.Pitch = RotPitch;
}

void UGravityGunCameraShake::SetRotYaw(float Amplitude, float Frequency)
{
	//Setting new values for the roational yaw of the oscillation
	RotYaw.Amplitude = Amplitude;
	RotYaw.Frequency = Frequency;

	RotOscillation.Pitch = RotYaw;

}

UGravityGunCameraShake::UGravityGunCameraShake()
{

	//Default values being applied for both pitch and yaw rotational oscillation
	RotPitch.Amplitude = 1.0f;
	RotPitch.Frequency = FMath::RandRange(15.0f, 50.0f);

	RotYaw.Amplitude = 1.0f;
	RotYaw.Frequency = FMath::RandRange(15.0f, 50.0f);

	RotOscillation.Pitch = RotPitch;
	RotOscillation.Yaw = RotYaw;

	//Setting the duration of the oscillation
	OscillationDuration = .5f;
}

void UGravityGunCameraShake::PlayShake(APlayerCameraManager* Camera, float Scale, ECameraAnimPlaySpace::Type InPlaySpace, FRotator UserPlaySpaceRot)
{
	Super::PlayShake(Camera, Scale, InPlaySpace, UserPlaySpaceRot);
}

void UGravityGunCameraShake::UpdateAndApplyCameraShake(float DeltaTime, float Alpha, FMinimalViewInfo& InOutPOV)
{
	Super::UpdateAndApplyCameraShake(DeltaTime, Alpha, InOutPOV);
}