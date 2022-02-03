// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "WeaponPickup.h"
#include "WeaponSpawner.generated.h"

/**
 * 
 */
UCLASS()
class KINGOFTHEGUN_API AWeaponSpawner : public ATargetPoint
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AWeaponSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Spawning")
	float timerMin = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Spawning")
	float timerMax = 5.0f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Spawning")
	TArray<TSubclassOf<AWeaponPickup>> WeaponList;

	UPROPERTY(EditAnywhere, Category = Movement)
	float hoverDistance;
	UPROPERTY(EditAnywhere, Category = Movement)
	float hoverSpeed;
	UPROPERTY(EditAnywhere, Category = Movement)
	float rotationSpeed;;
	UPROPERTY(EditAnywhere, Category = Movement)
	bool canHover;
	UPROPERTY(EditAnywhere, Category = Movement)
	float hoverDistanceWithFloor;
private:
	//AActor* weaponSpawned;
	TWeakObjectPtr<AWeaponPickup> weaponSpawned;
	float timer;
	float timerHover;
	float myRotation;
	
};
