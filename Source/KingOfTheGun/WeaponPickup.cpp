// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"
#include "KingOfTheGunCharacter.h"

// Sets default values
AWeaponPickup::AWeaponPickup()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	PrimaryActorTick.bCanEverTick = true;
	//SetRootComponent(Cast<USceneComponent>(this));
	active = true;
	bullets = 10;
	ammoType = 0;
	gunType = 0;
	WeaponRange = 20000;

	hoverDistance = 5.0f;
	hoverDistanceWithFloor = 40.0f;
	hoverSpeed = 1.0f;;
	rotationSpeed = 0.5f;
	timeToGoAtHoverPostion = 2.0f;
	timeOnGround = 2.0f;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		NewMesh(TEXT("SkeletalMesh'/Game/SciFiWeapDark/Weapons/Darkness_Pistol.Darkness_Pistol'"));
	mesh->SetSkeletalMesh(NewMesh.Object);
	mesh->SetupAttachment(RootComponent);
	mesh->SetGenerateOverlapEvents(true);
	mesh->SetSimulatePhysics(true);
	mesh->SetCollisionProfileName(TEXT("Weapon"));
}

// Called when the game starts or when spawned
void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();

	isHovering = false;
	timerHover = 0;
}

void AWeaponPickup::Tick(float DeltaTime)
{
	if (isHovering && this->mesh->IsValidLowLevel()) {
		
		FVector newLocation = this->mesh->GetRelativeLocation();
		float deltaHeight;
		if (timerHover < timeOnGround) {
			//rien, quand arme est au sol
		}
		else if (timerHover < timeToGoAtHoverPostion + timeOnGround) {
			if (!hoverSettingsAreSet) {
				mesh->SetSimulatePhysics(false);
				mesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
				hoverSettingsAreSet = true;
			}
			deltaHeight = hoverDistanceWithFloor * DeltaTime / timeToGoAtHoverPostion;
			this->AddActorLocalRotation(FRotator(0, rotationSpeed, 0));
		}
		else {
			deltaHeight = (FMath::Sin(timerHover + DeltaTime * hoverSpeed) - FMath::Sin(timerHover))* hoverDistance;
			this->AddActorLocalRotation(FRotator(0, rotationSpeed, 0));
		}
		newLocation.Z += deltaHeight;


		timerHover += DeltaTime * hoverSpeed;

		this->mesh->SetRelativeLocation(newLocation);
	}
}

void AWeaponPickup::startHover()
{
	isHovering = true;
	hoverSettingsAreSet = false;
	timerHover = 0;
}

void AWeaponPickup::startInstantHover()
{
	isHovering = true;
	hoverSettingsAreSet = false;

	mesh->SetSimulatePhysics(false);
	mesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	hoverSettingsAreSet = true;

	timerHover = timeToGoAtHoverPostion + timeOnGround;

	FVector newLocation = this->mesh->GetRelativeLocation();
	newLocation.Z += hoverDistanceWithFloor;
	this->mesh->SetRelativeLocation(newLocation);
}

void AWeaponPickup::Init()
{
	SetRootComponent(Cast<USceneComponent>(this));
}