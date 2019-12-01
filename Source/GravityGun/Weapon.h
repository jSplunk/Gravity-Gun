// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Weapon.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWeapon, Log, All);

/*
 *
 *	Abstract weapon class inherited from inventory items
 *
 */

UCLASS()
class GRAVITYGUN_API AWeapon : public AInventoryItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	
	/*
		Protected properties of the weapon
	*/

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Gun muzzle's offset from the characters location, used to determine where the shots are spawned from
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (DisplayName = "Weapon Offset"))
	FVector GunOffset;

	// Sound to play each time we fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	// Sound to play each time we use secondary fire 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* SecondaryFireSound;

	//Location on gun mesh where the items are pulled towards
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (DisplayName = "Weapon Muzzle"))
	class USceneComponent* MuzzleLocation;

public:

	/*
		Public virtual functions for inherited classes
	*/

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Implements a virtual fire method for derived classes
	virtual void Fire() PURE_VIRTUAL(AWeapon::Fire, );

	//Implements a virtual secondary fire method for derived classes, can also be aim down sights
	virtual void SecondaryFire() PURE_VIRTUAL(AWeapon::SecondaryFire, );

	//Implements a virtual function for setting properties when being picked up
	virtual void SetPickupProperties() PURE_VIRTUAL(AWeapon::SetPickupProperties, );
	
};
