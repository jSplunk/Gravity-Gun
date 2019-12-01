// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryGameMode.h"
#include "GravityGunHUD.h"
#include "InventoryCharacter.h"
#include "UObject/ConstructorHelpers.h"


AInventoryGameMode::AInventoryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("Blueprint'/Game/GravityGun/BPGravityChar'"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGravityGunHUD::StaticClass();
}

