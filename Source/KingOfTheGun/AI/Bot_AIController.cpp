// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"
#include "../KingOfTheGunProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

ABot_AIController::ABot_AIController()
{
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ABot_AIController::OnPawnDetected);

	projectileSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Projectile Detection Sphere"));
	projectileSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	projectileSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	projectileSphere->OnComponentBeginOverlap.AddDynamic(this, &ABot_AIController::OnProjectileOverlap);
	projectileSphere->SetSphereRadius(200.0f);
	
}

void ABot_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Some Problem Occured"));
	}

}

void ABot_AIController::OnPossess(APawn* InPawn) 
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComp->StartTree(*BehaviorTree);
	}
	AIChar = Cast<AKingOfTheGunCharacter>(this->GetCharacter());
	
	AIChar->interactRange->OnComponentBeginOverlap.AddDynamic(this, &ABot_AIController::OnWeaponOverlap);
	/*
	projectileSphere->RegisterComponent();
	projectileSphere->AttachToComponent(AIChar->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	*/

}

void ABot_AIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GetEnemy();
	AdjustFPGun();

	if (this->AIChar->GetHealth() <= 0) {
		this->AIChar->GetCharacterMovement()->StopActiveMovement();
		this->AIChar->StopFire();
	}
}

FRotator ABot_AIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	if (this->enemy != nullptr) {
		FVector headLocation = this->enemy->GetMesh()->GetSocketLocation("head");
		return UKismetMathLibrary::FindLookAtRotation(this->AIChar->GetActorLocation(), headLocation);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);

}

void ABot_AIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (AActor* Actor : DetectedPawns)
	{
		if (Actor->IsA<AKingOfTheGunCharacter>() && !GetSeeingPawn())
		{
			BlackboardComp->SetValueAsObject(BBEnemyKey, Actor);
			SetFocus(Actor);
			return;
		}
		if (Actor->IsA<AKingOfTheGunProjectile>())
		{
			BlackboardComp->SetValueAsObject(FName("projectile"), Actor);
			return;
		}
	}

	BlackboardComp->SetValueAsObject(BBEnemyKey, nullptr);
	ClearFocus(EAIFocusPriority::Gameplay);
}

AActor* ABot_AIController::GetSeeingPawn() {
	UObject* object = BlackboardComp->GetValueAsObject(BBEnemyKey);

	return object ? Cast<AActor>(object) : nullptr;
}

void ABot_AIController::EquipWeapon() {
	//BlackboardComp->SetValueAsBool(BBWeaponEquippedKey, true);
}

void ABot_AIController::OnProjectileOverlap(class UPrimitiveComponent* OtherComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->IsA<AKingOfTheGunProjectile>())
	{
		BlackboardComp->SetValueAsObject(FName("projectile"), OtherActor);
		return;
	}

	BlackboardComp->SetValueAsObject(FName("projectile"), nullptr);
}

void ABot_AIController::OnWeaponOverlap(class UPrimitiveComponent* OtherComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - this->timeLastWeaponSwitch >= 3) {
		this->timeLastWeaponSwitch = UGameplayStatics::GetRealTimeSeconds(GetWorld());

		this->AIChar->Interact();
		
		UE_LOG(LogTemp, Warning, TEXT("Equip weapon"));
	}
}

void ABot_AIController::GetEnemy()
{
	TArray<AActor*> allPlayers;
	if (allPlayers.Num() <= 0) {
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKingOfTheGunCharacter::StaticClass(), allPlayers);
	}

	for (AActor* myTarget : allPlayers) {
		if (!(myTarget == this->AIChar) && this->LineOfSightTo(myTarget) && !(Cast<AKingOfTheGunCharacter>(myTarget)->GetIsDead()) && (this->AIChar->GetDistanceTo(myTarget) < 1500.f)) {
			this->enemy = Cast<AKingOfTheGunCharacter>(myTarget);

			BlackboardComp->SetValueAsObject(BBEnemyKey, this->enemy);
			SetFocus(this->enemy);
			return;
		}
	}

	this->enemy = nullptr;
	BlackboardComp->SetValueAsObject(BBEnemyKey, nullptr);
	ClearFocus(EAIFocusPriority::Gameplay);
}

void ABot_AIController::StartFire() {
	this->AIChar->StartFire();

	this->timeLastShot = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	//this->AIChar->GetCharacterMovement()->Activate();
}

void ABot_AIController::AdjustFPGun() {
	FVector tpGunLocation = AIChar->TP_Gun->GetComponentLocation();
	FRotator tpGunRotation = AIChar->TP_Gun->GetComponentRotation();
	AIChar->FP_Gun->SetWorldLocationAndRotation(tpGunLocation, tpGunRotation.Quaternion());
}