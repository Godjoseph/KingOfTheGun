// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Fire.generated.h"

UCLASS()
class KINGOFTHEGUN_API AFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFire();

public:	
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* Fire;

	UPROPERTY(EditAnywhere)
		UBoxComponent* MyBoxComponent;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> FireDamageType;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> MyCharacters;

	UPROPERTY(EditAnywhere)
		FHitResult MyHit;

	UPROPERTY(EditAnywhere)
		float fireDamage;

	bool bCanApplyDamage;
	FTimerHandle FireTimerHandle;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
						  int32 OtherBodyIndex);

	UFUNCTION()
		void ApplyFireDamage();


};
