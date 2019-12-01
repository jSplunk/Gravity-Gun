// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryCharacter.h"
#include "Inventory.h"
#include "InventoryItem.h"
#include "InventoryPlayerController.h"
#include "Animation/AnimInstance.h"
#include "WeaponGravityGun.h"
#include "Camera/CameraComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "ConstructorHelpers.h"

// Sets default values
AInventoryCharacter::AInventoryCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInventoryCharacter::OnBeginOverlap);

	// set our turn rates for input
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	// Create a CameraComponent	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCamera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Char = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character"));
	Char->SetupAttachment(FirstPersonCamera);
	Char->bCastDynamicShadow = false;
	Char->CastShadow = false;
	Char->RelativeRotation = FRotator(1.700328f, -17.010868f, 5.268492f);
	Char->RelativeLocation = FVector(-2.663825f, -1.518556f, -155.982254f);
	Char->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Char->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void AInventoryCharacter::BeginPlay()
{
	Super::BeginPlay();

	Char->SetHiddenInGame(true);
	bIsHiddenMesh = true;
	CharacterInventory->SetCamera(FirstPersonCamera);

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//GravityGun->AttachToComponent(Char, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	//GravityGun->GetInventoryItemSkeletalMesh()->AttachToComponent(Char, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void AInventoryCharacter::PickUpInventoryItem()
{
	CharacterInventory->PickupItem();
	if (CharacterInventory->LastItemSeen)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Gun Here!"));
		//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
		Cast<AWeapon>(CharacterInventory->GetEquippedItem())->AttachToComponent(Char, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		Cast<AWeapon>(CharacterInventory->GetEquippedItem())->SetPickupProperties();
	}
}

void AInventoryCharacter::DropInventoryItem()
{
	CharacterInventory->DropEquippedItem(this);
	Char->SetHiddenInGame(true);
	bIsHiddenMesh = true;
}

void AInventoryCharacter::HandleInventoryInput()
{
	AInventoryPlayerController* Con = Cast<AInventoryPlayerController>(GetController());
	if (Con) Con->HandleInventoryInput();
}


void AInventoryCharacter::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		// add movement based on the ForwardVector (Forward and Backwards)
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void AInventoryCharacter::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		// add movement based on the RightVector (Right and Left)
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void AInventoryCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AInventoryCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AInventoryCharacter::Fire()
{
	if (CharacterInventory->GetEquippedItem())
		Cast<AWeapon>(CharacterInventory->GetEquippedItem())->Fire();
}

void AInventoryCharacter::SecondaryFire()
{
	if(CharacterInventory->GetEquippedItem())
		Cast<AWeapon>(CharacterInventory->GetEquippedItem())->SecondaryFire();
}

// Called every frame
void AInventoryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharacterInventory) CharacterInventory->Raycast(this);

	if (!CharacterInventory->GetEquippedItem() && bIsHiddenMesh)
	{
		Char->SetHiddenInGame(true);
		bIsHiddenMesh = true;
	}
}

// Called to bind functionality to input
void AInventoryCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Bind Fire and Secondary Fire
	PlayerInputComponent->BindAction("Launch", IE_Pressed, this, &AInventoryCharacter::Fire);
	PlayerInputComponent->BindAction("Attract", IE_Pressed, this, &AInventoryCharacter::SecondaryFire);

	//Drop currently equipped item
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &AInventoryCharacter::DropInventoryItem);

	//Pick up an item for inventory
	PlayerInputComponent->BindAction("PickUpItem", IE_Pressed, this, &AInventoryCharacter::PickUpInventoryItem);

	//Action mapping for open inventory
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AInventoryCharacter::HandleInventoryInput);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AInventoryCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AInventoryCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AInventoryCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AInventoryCharacter::LookUpAtRate);

}

void AInventoryCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//If the overlapped item is a weapon
		if(OtherActor->IsA<AWeapon>())
		{
			//Enable the skeletal mesh for the character
			Char->SetHiddenInGame(false);
			bIsHiddenMesh = false;

			//Cast to item so we can store the actor as an inventory item
			AInventoryItem* Item = Cast<AInventoryItem>(OtherActor);

			//Sets the last seen item to the one we found
			CharacterInventory->LastItemSeen = Item;

			//Picking up the item to the inventory
			PickUpInventoryItem();

			//When we equip the weaopn, it needs to be adjusted booth location and rotation, and also disable physics
			//Needs to be changed if there are multiple guns, since their adjustements are diffrent
			//Would need to incorporate another property of the gun

			if (CharacterInventory->LastItemSeen)
			{
				Cast<AWeapon>(CharacterInventory->GetEquippedItem())->SetPickupProperties();

				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Gun Here!"));
				//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
				Cast<AWeapon>(CharacterInventory->GetEquippedItem())->AttachToComponent(Char, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			}
		}
		else if(OtherActor->IsA<AInventoryItem>())
		{
			AInventoryItem* Item = Cast<AInventoryItem>(OtherActor);

			CharacterInventory->LastItemSeen = Item;
			PickUpInventoryItem();
		}

	}
}

//void AInventoryCharacter::OnEndOverap(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
//{
//}

