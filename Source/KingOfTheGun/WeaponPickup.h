// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponPickup.generated.h"

UCLASS()
class KINGOFTHEGUN_API AWeaponPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	bool canHover;

public:
	bool active;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* SB_GunSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int bullets;

	//NORMAL = 0, GRENADE = 1, SNIPER = 2, STICKY = 2, WIND = 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ammoType;
	
	//PISTOL = 0, RIFLE = 1, ROCKET = 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int gunType;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bulletSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float gunDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float headshotDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float minimumRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maximumRecoil;

	/** The seconds to wait between shots*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float TimeBetweenShots;

	/** The seconds to wait between shots*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isLineTracing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isShotgun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isPistol;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class USkeletalMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = Movement)
		float hoverDistance;
	UPROPERTY(EditAnywhere, Category = Movement)
		float hoverSpeed;
	UPROPERTY(EditAnywhere, Category = Movement)
		float rotationSpeed;;	
	UPROPERTY(EditAnywhere, Category = Movement)
		float hoverDistanceWithFloor;
	UPROPERTY(EditAnywhere, Category = Movement)
		float timeToGoAtHoverPostion;
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void startHover();
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void startInstantHover();
	void Init();

private:
	bool isHovering;
	bool hoverSettingsAreSet;
	float timerHover;
	float timeOnGround;

};
