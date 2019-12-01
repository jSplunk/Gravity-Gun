// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "InventoryItem.h"
#include "Camera/CameraComponent.h"
#include "Weapon.h"
#include "WeaponGravityGun.h"
#include "Sound/SoundCue.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "InventoryCharacter.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"

DEFINE_LOG_CATEGORY(LogInventory);

// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LastItemSeen = nullptr;

	LatestAvailablePositionInInventory = 0;

	//Initializing our Inventory
	Inventory.Reserve(MAX_INVENTORY_ITEMS);
	Inventory.SetNum(MAX_INVENTORY_ITEMS);

}

void AInventory::SetCamera(UCameraComponent* camera)
{
	Camera = camera;
}

void AInventory::PickupItem()
{
	int key2 = 12;
	int key = 9;

	if (LastItemSeen)
	{
		//Find the first available slot
		int32 AvailableSlot = Inventory.Find(nullptr);

		if (AvailableSlot < MAX_INVENTORY_ITEMS && AvailableSlot != -1)
		{
			GEngine->AddOnScreenDebugMessage(key, 2.0f, FColor::Blue, "Picked up Item!");
			//Add the item to the first valid slot we found
			Inventory[AvailableSlot] = LastItemSeen;

			EquippedItem = Inventory[AvailableSlot];

			LatestAvailablePositionInInventory++;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(key2, 2.0f, FColor::Blue, "Inventory is full!");
		}
		
	}
}

void AInventory::DropEquippedItem(AActor* Player)
{
	if (EquippedItem)
	{
		int32 IndexOfItem;
		if (Inventory.FindLast(EquippedItem, IndexOfItem))
		{
			//The location of the drop
			FVector DropLocation = Cast<AInventoryCharacter>(Player)->GetCharacterCamera()->GetComponentLocation() + (Cast<AInventoryCharacter>(Player)->GetActorForwardVector() * 400);

			//Making a transform with default rotation and scale. Just setting up the location
			//that was calculated above
			FTransform Transform; Transform.SetLocation(DropLocation);
			Transform.SetRotation(FQuat(0, 0, -45, 0));

			//Default actor spawn parameters
			FActorSpawnParameters SpawnParams;

			if (EquippedItem->IsA<AWeaponGravityGun>())
			{
				//Spawning our pickup
				AWeaponGravityGun* PickupToSpawn = GetWorld()->SpawnActor<AWeaponGravityGun>(EquippedItem->GetClass(), Transform, SpawnParams);
				

				if (PickupToSpawn)
				{
					//Unreference the item we've just placed
					Inventory[IndexOfItem] = nullptr;
					LatestAvailablePositionInInventory--;
					EquippedItem->Destroy();
					EquippedItem = nullptr;
				}
			}
			else if (EquippedItem->IsA<AWeapon>())
			{
				//Spawning our pickup
				AWeapon* PickupToSpawn = GetWorld()->SpawnActor<AWeapon>(EquippedItem->GetClass(), Transform, SpawnParams);

				if (PickupToSpawn)
				{
					//Unreference the item we've just placed
					Inventory[IndexOfItem] = nullptr;
					LatestAvailablePositionInInventory--;
					EquippedItem->Destroy();
					EquippedItem = nullptr;
				}
			}
			else
			{
				//Spawning our pickup
				AInventoryItem* PickupToSpawn = GetWorld()->SpawnActor<AInventoryItem>(EquippedItem->GetClass(), Transform, SpawnParams);

				if (PickupToSpawn)
				{
					//Unreference the item we've just placed
					Inventory[IndexOfItem] = nullptr;
					LatestAvailablePositionInInventory--;
					EquippedItem->Destroy();
					EquippedItem = nullptr;
				}
			}

			


			
		}
	}
}

void AInventory::Raycast(AActor* Player)
{
	if (Camera == nullptr)
	{
		UE_LOG(LogInventory, Verbose, TEXT("AInventory->Camera is a nullptr, cannot get camera location"));
		return;
	}

	//Calculating start and end location
	FVector StartLocation = Camera->GetComponentLocation();
	FVector EndLocation = StartLocation + (Camera->GetForwardVector() * RaycastRange);

	//Draw line from start of raycast to end of raycast location
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red);

	FHitResult RaycastHit;

	//Raycast should ignore the character
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(Player);
	CQP.AddIgnoredActor(this);

	int key = 14;

	//Raycast
	if (GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, CQP))
	{
		USceneComponent* Pickup = Cast<USceneComponent>(RaycastHit.Component);
		if (Pickup)
		{
			GEngine->AddOnScreenDebugMessage(key, 2.0f, FColor::Blue, Pickup->GetName());

			AActor* Item = Pickup->GetOwner();

			if(Item->IsA<AInventoryItem>())
			{
				GEngine->AddOnScreenDebugMessage(key, 2.0f, FColor::Blue, "Seeing item to pick up!");
				//Set the last seen item to the hit item
				LastItemSeen = Cast<AInventoryItem>(Pickup->GetOwner());
			}
			else LastItemSeen = nullptr;
			
		}
		else //Re-Initialize 
			LastItemSeen = nullptr;
	}


}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!EquippedItem && LastItemSeen && LatestAvailablePositionInInventory != 0) EquippedItem = Inventory[LatestAvailablePositionInInventory - 1];
}

