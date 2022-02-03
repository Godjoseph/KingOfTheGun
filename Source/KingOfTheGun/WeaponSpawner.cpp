// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSpawner.h"
#include "Kismet/GameplayStatics.h"
#include <random>

AWeaponSpawner::AWeaponSpawner() {
	hoverDistance = 5.0f;
	hoverDistanceWithFloor = 40.0f;
	hoverSpeed = 1.0f;;
	rotationSpeed = 0.5f;
	canHover = true;
}

void AWeaponSpawner::BeginPlay()
{
	PrimaryActorTick.bCanEverTick = true;
	Super::BeginPlay();
	weaponSpawned = nullptr;
	if (WeaponList.Num() > 0) {
		std::random_device rd;
		std::default_random_engine e1(rd());
		std::uniform_int_distribution<int> uniform_dist(0, WeaponList.Num()-1);
		std::uniform_real_distribution<float> uniform_dist_real(timerMin, timerMax);
		int32 monIndex = uniform_dist(e1);
		myRotation = uniform_dist_real(e1);
		timer = uniform_dist_real(e1);
		timerHover = uniform_dist_real(e1);
		//myRotation = uniform_dist_real_rotate(e1);
	}

}

void AWeaponSpawner::Tick(float DeltaTime)
{
	if (weaponSpawned.IsValid()){
		/*if (canHover && weaponSpawned->mesh->IsValidLowLevel()) {
			weaponSpawned->AddActorLocalRotation(FRotator(0, rotationSpeed, 0));
			FVector newLocation = weaponSpawned->mesh->GetRelativeLocation();
			float deltaHeight = (FMath::Sin(timerHover + DeltaTime* hoverSpeed) - FMath::Sin(timerHover));
			newLocation.Z += deltaHeight * hoverDistance;


			timerHover += DeltaTime * hoverSpeed;

			weaponSpawned->mesh->SetRelativeLocation(newLocation);
		}*/

	}
	else {
		timer -= DeltaTime;
		if (WeaponList.Num() > 0 && timer < 0) {
			timer = 2.0f;
			std::random_device rd;
			std::default_random_engine e1(rd());
			std::uniform_int_distribution<int> uniform_dist(0, WeaponList.Num() - 1);
			std::uniform_real_distribution<float> uniform_dist_real(timerMin, timerMax);
			int32 monIndex = uniform_dist(e1);
			FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
			weaponSpawned = Cast<AWeaponPickup>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), WeaponList[monIndex], SpawnTransform));
			if (weaponSpawned.Get())
			{
				weaponSpawned->Init();
				UGameplayStatics::FinishSpawningActor(weaponSpawned.Get(), SpawnTransform);
			}
			weaponSpawned->mesh->SetSimulatePhysics(!canHover);
			if (canHover) {
				/*FVector newLocation = weaponSpawned->GetActorLocation();
				newLocation.Z += hoverDistanceWithFloor + FMath::Sin(timerHover) * hoverDistance;
				weaponSpawned->mesh->SetRelativeLocation(newLocation);*/
				weaponSpawned->startInstantHover();
			}
			timer = uniform_dist_real(e1);
		}
	}
}
