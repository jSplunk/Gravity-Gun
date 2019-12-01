// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

DEFINE_LOG_CATEGORY(LogWeapon);

// Sets default values
AWeapon::AWeapon() : GunOffset(FVector(0)), MuzzleLocation(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

