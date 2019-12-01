// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.h"
#include "InventoryUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYGUN_API UInventoryUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/*Adds the widget into the viewport and populates the inventory slots*/
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void Show();

	/*Removes the widget from the viewport*/
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void Hide();

	/*Stores a reference in order to bind information on inventory slots*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AInventoryItem*> ItemsArray;
	
	
};
