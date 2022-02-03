// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KingOfTheGunCharacter.h"
#include "Grenade.generated.h"

class UNiagaraSystem;
class USoundBase;

UCLASS()
class KINGOFTHEGUN_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	class USkeletalMeshComponent* GrenadeMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	class USphereComponent* DamageSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	float FuseLength;

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	class UAnimationAsset* ThrowAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	UNiagaraSystem* NS_Explosion;

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	USoundBase* SB_Explosion;

	FVector ForwardVectorToApply;
public:

	void OnRelease(AKingOfTheGunCharacter* CharacterOwner, FVector ForwardVector);
	void ContinueOnRelease();
	void Explode();

	UFUNCTION(BlueprintImplementableEvent)
	void DamageWall(AActor* actor, FVector hit);

private:

	AKingOfTheGunCharacter* GrenadeOwner;
};
