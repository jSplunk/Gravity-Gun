// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "InventoryCharacter.h"
#include "Inventory.h"
#include "InventoryUserWidget.h"

void AInventoryPlayerController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	if (InventoryWidgetBP)
	{
		InventoryWidgetRef = CreateWidget<UInventoryUserWidget>(this, InventoryWidgetBP);
	}

	bIsInventoryOpen = false;
}

void AInventoryPlayerController::HandleInventoryInput()
{
	AInventoryCharacter* Character = Cast<AInventoryCharacter>(GetPawn());

	if (InventoryWidgetRef)
	{
		if (bIsInventoryOpen)
		{
			bIsInventoryOpen = false;

			InventoryWidgetRef->RemoveFromViewport();
		}
		else
		{
			bIsInventoryOpen = true;

			InventoryWidgetRef->ItemsArray = Character->GetCharacterInventory()->GetInventory();

			InventoryWidgetRef->Show();
		}
	}

}
