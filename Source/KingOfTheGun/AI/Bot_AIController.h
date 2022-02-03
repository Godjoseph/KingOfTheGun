// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/SphereComponent.h"
#include "Bot_AIController.generated.h"

class AKingOfTheGunCharacter;
class APathingPoint;

UCLASS()
class KINGOFTHEGUN_API ABot_AIController : public AAIController
{
	GENERATED_BODY()
private:
	UBehaviorTreeComponent* BehaviorTreeComp;
	const FName BBEnemyKey = FName("Enemy");
	const FName BBItemKey = FName("ItemToGet");
	const FName BBWeaponEquippedKey = FName("WeaponEquipped");
	AKingOfTheGunCharacter* AIChar;
	AKingOfTheGunCharacter* enemy;
protected:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	USphereComponent* projectileSphere;
public:
	ABot_AIController();

	UBlackboardComponent* BlackboardComp;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void OnPossess(APawn* InPawn) override;

	virtual FRotator GetControlRotation() const override;

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	UFUNCTION()
	void OnProjectileOverlap(class UPrimitiveComponent* OtherComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponOverlap(class UPrimitiveComponent* OtherComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EquipWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRadius = 50000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRadius = AISightRadius + 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 50000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	bool bIsPlayerDetected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float DistanceToPlayer = 0.0f;

	AActor* GetSeeingPawn();

	float timeSinceLastHit = 0;
	float timeLastShot = 0;
	float timeLastWeaponSwitch = 0;

	APathingPoint* currentPathingPoint;
	int threat;
	void GetEnemy();
	void StartFire();
	void AdjustFPGun();
};
