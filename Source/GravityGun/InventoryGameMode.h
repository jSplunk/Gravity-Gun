// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InventoryGameMode.generated.h"

/**
 * Used as a game mode for supplying default pawn class
 */
UCLASS()
class GRAVITYGUN_API AInventoryGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AInventoryGameMode();
	
};
