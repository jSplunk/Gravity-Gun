// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AInventoryItem::AInventoryItem() : ItemName("")
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing the capsule collider for the items we are picking up
	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	CapsuleCollider->SetupAttachment(RootComponent);

	//Initializing our properties
	//The item can either have a skeletal mesh
	InventoryItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("InventoryItemSkeletalMesh"));
	InventoryItemSkeletalMesh->SetupAttachment(CapsuleCollider);

	//Or have a static mesh
	InventoryItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InventoryItemStaticMesh"));
	InventoryItemStaticMesh->SetupAttachment(CapsuleCollider);

}

// Called when the game starts or when spawned
void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

