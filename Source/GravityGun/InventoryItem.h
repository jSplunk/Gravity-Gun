// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItem.generated.h"

UCLASS()
class GRAVITYGUN_API AInventoryItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventoryItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*The Static Mesh of the pickup*/
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* InventoryItemStaticMesh;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleCollider;

	/*The Skeletal Mesh of the pickup*/
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* InventoryItemSkeletalMesh;

	//The Texture of the item in case we want to add it in the secrets or inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	class UTexture2D* ItemTexture;

	//The name of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FString ItemName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Returns the Texture of our Pickup
	FORCEINLINE UTexture2D* GetPickupTexture() { return ItemTexture; }

	FORCEINLINE UStaticMeshComponent* GetInventoryItemStaticMesh() { return InventoryItemStaticMesh; }

	FORCEINLINE USkeletalMeshComponent* GetInventoryItemSkeletalMesh() { return InventoryItemSkeletalMesh; }

	FORCEINLINE UCapsuleComponent* GetCapsuleCollider() { return CapsuleCollider; }

	FORCEINLINE void SetItemName(FString Name) { ItemName = Name; }

	FORCEINLINE FString GetItemName() { return ItemName; }
	
	
};