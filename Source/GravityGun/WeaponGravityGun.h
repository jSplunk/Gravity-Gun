// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponGravityGun.generated.h"

/**
 * This is an implementation of a gravity gun which allows you to pull items towards you.
 * Shoot them away from you, both while holding them and while they are on the ground.
 *
 * Works only on actors with "Simulate Physics" enabled, and when they are blocking the Camera
 * collision channel.
 */

UCLASS()
class GRAVITYGUN_API AWeaponGravityGun : public AWeapon
{
	GENERATED_BODY()


public:
	//Default contructor
	AWeaponGravityGun();

protected:

	//The range of the raycast
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Gravity Gun")
	float RaycastRange;

	//The range before attaching the object
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Gravity Gun")
	float AttachRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity Gun")
	int ImpulseForceWhenShootingTarget;

	//Used for pulling objects towards the gun
	UPROPERTY(BlueprintReadwrite, Category = "Gravity Gun")
	class UPhysicsHandleComponent* PhysicsHandle;

	//The pulled in object as an attachment to the gun
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attached Object")
	class USceneComponent* AttachedObject;

	//Checking if the object is attached to the gun
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attached Object")
	bool bIsAttachedObject;

	//Flag which checks when the gun is held by a player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attached Object")
	bool bIsHeldByPlayer;

	//Reference to a camera shake object
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShake> Shake;

public:

	/*
		Public virtual functions for inherited classes
	*/

	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called before Tick
	virtual void BeginPlay() override;

	//Overridden function from parent class, primary fire method
	UFUNCTION(BlueprintCallable, Category = "Gravity Gun")
	virtual void Fire() override;

	//Overridden function from parent class, secondary fire method
	UFUNCTION(BlueprintCallable, Category = "Gravity Gun")
	virtual void SecondaryFire() override;

	//Raycasts out in the scene to find which objects to pull in
	UFUNCTION(BlueprintCallable, Category = "Find Objects")
	virtual void FindObjects();

	//Detaches objects that are pulled in
	UFUNCTION(BlueprintCallable, Category = "Detach Object")
	virtual void DetachObject();

	//Setup specific properties when spawning a new gravity gun
	UFUNCTION(BlueprintCallable, Category = "Gravity Gun")
	virtual void SetPickupProperties() override;
};
