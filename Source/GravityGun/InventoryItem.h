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

	//Getters for components
	FORCEINLINE class UStaticMeshComponent* GetInventoryItemStaticMesh() { return InventoryItemStaticMesh; }
	FORCEINLINE class USkeletalMeshComponent* GetInventoryItemSkeletalMesh() { return InventoryItemSkeletalMesh; }
	FORCEINLINE class UCapsuleComponent* GetCapsuleCollider() { return CapsuleCollider; }
	FORCEINLINE FString GetItemName() { return ItemName; }

	//Setter for name
	FORCEINLINE void SetItemName(FString Name) { ItemName = Name; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The Static Mesh of the item
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* InventoryItemStaticMesh;

	//The Collider for the item
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleCollider;

	//The Skeletal Mesh of the pickup
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* InventoryItemSkeletalMesh;

	//The name of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FString ItemName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
