// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

#define MAX_INVENTORY_ITEMS 4

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

UCLASS()
class GRAVITYGUN_API AInventory : public AActor
{
	GENERATED_BODY()
private:
	/*
		Private variables
	*/

	//Camera pointer to component from character
	class UCameraComponent* Camera;

	//Reference to the last seen pickup item. Nullptr if none
	class AInventoryItem* LastItemSeen;

	//Keeps track on what amount of items are in the array, and the last position
	int LatestAvailablePositionInInventory;

public:

	/*
		Public class specific functions
	*/

	// Sets default values for this actor's properties
	AInventory();

	//Set the camera object pointer for inventory
	UFUNCTION()
	void SetCamera(UCameraComponent* camera);

	//Handles the picking up the item
	UFUNCTION()
	void PickupItem();

	//Drops the currently equipped item
	UFUNCTION()
	void DropEquippedItem(AActor* Player);

	//Raycasts in front of the character to find usable items
	UFUNCTION()
	void Raycast(AActor* Player);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE TArray<AInventoryItem*> GetInventory() { return Inventory; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE AInventoryItem* GetEquippedItem() { return EquippedItem; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE AInventoryItem* GetLastItemSeen() { return LastItemSeen; }

protected:

	/*
		Protected properties of the inventory
	*/

	//Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The range of the raycast
	UPROPERTY(EditAnywhere)
	float RaycastRange = 500.f;

	//Inventory actor reference
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	class AInventoryItem* EquippedItem;

	//The actual Inventory
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<AInventoryItem*> Inventory;

public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
