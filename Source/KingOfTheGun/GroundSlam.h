// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KingOfTheGunCharacter.h"
#include "Components/TimelineComponent.h"
#include "GroundSlam.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGOFTHEGUN_API AGroundSlam : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AGroundSlam();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	FVector Start;
	FVector End;
	AKingOfTheGunCharacter* Character;
	UAnimInstance* CharacterAnimInstance;

public:	

	UPROPERTY(EditDefaultsOnly, Category = "GroundSlam")
	class USphereComponent* DamageSphere;

	UPROPERTY(EditDefaultsOnly, Category = "GroundSlam")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "GroundSlam")
	class UAnimMontage* GroundSlamTravelMontage;

	UPROPERTY(EditDefaultsOnly, Category = "GroundSlam")
	class UAnimMontage* GroundSlamEndMontage;

	FTimeline MyTimeline;

	UPROPERTY(EditAnywhere, Category = "GroundSlam")
	UCurveFloat* GroundSlamCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundSlam")
	class UNiagaraSystem* ImpactNiagara;

	UPROPERTY(EditDefaultsOnly, Category = "GroundSlam")
	USoundBase* SB_Impact;

	UFUNCTION()
	void TimelineProgress(float Value);

	void OnImpact();
	void MovePlayer(AKingOfTheGunCharacter* CharacterOwner, UAnimInstance* AnimInstance, FVector end);

	UFUNCTION(BlueprintImplementableEvent)
	void DamageWall(AActor* actor, FVector hit);
};
