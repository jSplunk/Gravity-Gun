// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYGUN_API AInventoryPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	/*InventoryWidget reference*/
	class UInventoryUserWidget* InventoryWidgetRef;

	/*True if the inventory is currently open - false otherwise*/
	bool bIsInventoryOpen;

protected:

	/*InventoryWidget Blueprint reference*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryUserWidget> InventoryWidgetBP;

public:
	virtual void Possess(APawn* InPawn) override;

	/*Opens or closes the inventory*/
	void HandleInventoryInput();
	
	
};
