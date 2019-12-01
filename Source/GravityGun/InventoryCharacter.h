// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryCharacter.generated.h"
/*
 *
 * Character class implementation, based on the GravityGunCharacter, using less and more specific properties. Used for inventory
 * and Weapon implemented characters. Must use "Pawn" for collision preset, or ignore Visibility in Trace Responses (This is for the raycast to work with the visibility layer).
 *
 */


UCLASS()
class GRAVITYGUN_API AInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	bool bIsHiddenMesh;

public:
	// Sets default values for this character's properties
	AInventoryCharacter();

	//Event to pick up items for inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PickUpInventoryItem();

	//Event to pick up items for inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropInventoryItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE AInventory* GetCharacterInventory() { return CharacterInventory; }

	UFUNCTION(BlueprintCallable, Category = Camera)
	FORCEINLINE UCameraComponent* GetCharacterCamera() { return FirstPersonCamera; }

	/*Handles the Inventory Input by sending information to the controller*/
	UFUNCTION()
	void HandleInventoryInput();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Base turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	//Base look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	//Pawn mesh: 1st person view (arms; seen only by self)
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Char;

	//Camera for First Person perspective view
	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	class UCameraComponent* FirstPersonCamera;

	//Inventory actor reference
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	class AInventory* CharacterInventory;

	//Handles moving forward/backward
	void MoveForward(float Val);

	//Handles stafing movement, left and right
	void MoveRight(float Val);

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

	//Local caller fucntions for weapon firing ability
	void Fire();
	void SecondaryFire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Used for picking up inventory items
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*virtual void OnEndOverap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/

};
