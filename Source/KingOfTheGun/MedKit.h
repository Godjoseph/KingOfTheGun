// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KingOfTheGunCharacter.h"
#include "MedKit.generated.h"

UCLASS()
class KINGOFTHEGUN_API AMedKit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMedKit();

public:	
	UFUNCTION()
		void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere)
		AKingOfTheGunCharacter* MyCharacter;

	UPROPERTY(EditAnywhere)
		float healthHealing;

	UPROPERTY(EditDefaultsOnly, Category = "Medkit")
		USoundBase* SB_Medkit;
};
