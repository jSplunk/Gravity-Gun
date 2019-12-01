// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponGravityGun.h"
#include "Weapon.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "GravityGunCameraShake.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "InventoryItem.h"
#include "DrawDebugHelpers.h"

AWeaponGravityGun::AWeaponGravityGun()
{
	//Create a gun mesh component
	GetCapsuleCollider()->InitCapsuleSize(20.f, 40.0f);
	GetCapsuleCollider()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetCapsuleCollider()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetInventoryItemSkeletalMesh()->bCastDynamicShadow = false;
	GetInventoryItemSkeletalMesh()->CastShadow = false;
	GetInventoryItemSkeletalMesh()->SetRelativeRotation(FRotator(0, 0, -90));
	GetInventoryItemSkeletalMesh()->SetRelativeLocation(FVector(0, 8, -13));
	
	//Create a muzzle location component
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GetInventoryItemSkeletalMesh());
	MuzzleLocation->SetRelativeLocation(FVector(0, 400.0f, 10.0f));

	//Creating physics handle
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	//Setting offset where the object will attach to the gun
	GunOffset = FVector(100.0f, 0.0f, 0.0f);

	//Range of the raycast
	RaycastRange = 2500.0f;

	//Distance away before we attach the object to the gun
	AttachRange = 700.0f;

	//Impulse force applied when shooting an object
	ImpulseForceWhenShootingTarget = 5000;

	bIsHeldByPlayer = false;

	
}

void AWeaponGravityGun::Tick(float DeltaTime)
{
	// Set the object's rotation and location every tick.
	if (bIsAttachedObject && AttachedObject)
	{
		//Setting the location of where the mesh will be attached to when we pull it in
		PhysicsHandle->SetTargetLocationAndRotation(MuzzleLocation->GetComponentLocation(), GetInventoryItemSkeletalMesh()->GetComponentRotation());
	}

	//Looking for objects that we can pull
	if(bIsHeldByPlayer)
		FindObjects();
}

void AWeaponGravityGun::BeginPlay()
{
	Super::BeginPlay();
	
	bIsAttachedObject = false;
}

void AWeaponGravityGun::Fire()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Fire"));

	//If the object is attached, shoot it away. Else detect if an object is in range
	DetachObject();
	//Apply Impulse to object identified by the raycast
	if (AttachedObject)
	{
		Cast<UPrimitiveComponent>(AttachedObject)->AddImpulse(ImpulseForceWhenShootingTarget * GetInventoryItemSkeletalMesh()->GetRightVector(), NAME_None, true);

		//Play a camera shake when we shoot away the object we are holding or objects we are hitting with the raycast
		if (Shake != nullptr)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(Shake);
		}
		AttachedObject = nullptr;
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		//This work is licensed under a Creative Commons Attribution 3.0 Unported License.
		//https://freesound.org/s/196907/
		//Sound by Dpoggioli on Freesound.org
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void AWeaponGravityGun::SecondaryFire()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Secondary Fire"));
	//If an object is attached, drop it
	if (bIsAttachedObject)
	{
		DetachObject();
		AttachedObject = nullptr;
	}
	else if (AttachedObject)
	{
		// try and play the sound if specified
		if (SecondaryFireSound != NULL)
		{
			//This work is licensed under a Creative Commons Attribution 3.0 Unported License.
			//https://freesound.org/s/51465/
			//Sound by smcameron on Freesound.org
			UGameplayStatics::PlaySoundAtLocation(this, SecondaryFireSound, GetActorLocation());
		}

		//Checking if the component is in range for pick up
		if (GetDistanceTo(AttachedObject->GetOwner()) > AttachRange)
		{
			/*
				If it isn't we apply a inverse force relative to the gun, so that the object is attacted by the gun.

				Because the direction is inverted, when you look at a valid object to attach, and then quickly look away and using secondary fire,
				that object still registers as an attached object, and it will go in the inverse direction of the gun mesh. 

				--Example, If you are looking to the right really fast it will go to the left--
			*/
			Cast<UPrimitiveComponent>(AttachedObject)->AddImpulse(ImpulseForceWhenShootingTarget / 5 * (GetInventoryItemSkeletalMesh()->GetRightVector() * -1), NAME_None, true);

			//Makes sure that we only apply the force once per update, since we don't want the item to fly all over the place when we arent looking at it
			AttachedObject = nullptr;
		}
		else
		{
			// Attach the object keeping the world transform of the object and shut down physics on it.
			PhysicsHandle->GrabComponentAtLocationWithRotation(Cast<UPrimitiveComponent>(AttachedObject), TEXT(""), AttachedObject->GetComponentLocation(), AttachedObject->GetComponentRotation());
			bIsAttachedObject = true;
			//Spawn particle effect and sound at location
		}
	}

}

void AWeaponGravityGun::FindObjects()
{
	//Calculating start and end location
	FVector StartLocation = GetInventoryItemSkeletalMesh()->GetSocketLocation("Ammo");
	FVector EndLocation = StartLocation + (GetInventoryItemSkeletalMesh()->GetRightVector() * RaycastRange);

	//Draw line from start of raycast to end of raycast location
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red);

	//HitResult for storing the Raycast hit
	FHitResult RaycastHit;

	//Raycast should ignore the character
	FCollisionQueryParams traceParams = FCollisionQueryParams("GravityGunTrace", true, this);

	//Used to not print out an unique message more than once
	int key = 32;

	//Raycast
	if (GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, traceParams))
	{
		USceneComponent* Pickup = RaycastHit.GetActor()->GetRootComponent();

		//Checking if the component is simulating physics
		if (Pickup->IsSimulatingPhysics())
		{
			GEngine->AddOnScreenDebugMessage(key, 2.0f, FColor::Yellow, TEXT("Found item to pick up"));
			AttachedObject = Pickup; //We attach it to our gun
		}
		//Makes sure that we don't attach an object when we aren't looking at it
		//Keeps it attached if we already attached it
		else if(!Pickup->IsSimulatingPhysics() && !bIsAttachedObject)
		{
			AttachedObject = nullptr;
		}
	}
}

void AWeaponGravityGun::DetachObject()
{
	if (bIsAttachedObject)
	{
		// Detach the object preserving the current world transform and turn the physics back on.
		PhysicsHandle->ReleaseComponent();
		bIsAttachedObject = false;

		// Spawn particle effect and sound at location
	}
}

void AWeaponGravityGun::SetPickupProperties()
{
	this->GetCapsuleCollider()->SetSimulatePhysics(false);
	this->SetActorRelativeLocation(FVector(-0.253597f, 10.116977f, 11.779691));
	this->SetActorRelativeRotation(FRotator(3.65411f, 0.97091f, 90.317802f));
	bIsHeldByPlayer = true;
}
