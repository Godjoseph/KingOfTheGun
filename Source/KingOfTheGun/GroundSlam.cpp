// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundSlam.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
AGroundSlam::AGroundSlam()
{
	PrimaryActorTick.bCanEverTick = true;
	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	DamageSphere->SetupAttachment(RootComponent);
}

// Called when the game starts
void AGroundSlam::BeginPlay()
{
	Super::BeginPlay();
	if (GroundSlamCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		MyTimeline.AddInterpFloat(GroundSlamCurve, TimelineProgress);
		MyTimeline.SetLooping(true);
	}
}

void AGroundSlam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyTimeline.TickTimeline(DeltaTime);
}

void AGroundSlam::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(Start, End, Value);
	bool Moved = Character->SetActorLocation(NewLocation, true);
	if (CharacterAnimInstance)
	{
		if (GroundSlamTravelMontage)
		{
			CharacterAnimInstance->Montage_Play(GroundSlamTravelMontage);
		}
	}
	if (!Moved)
	{
		if (CharacterAnimInstance)
		{
			if (GroundSlamEndMontage)
			{
				CharacterAnimInstance->Montage_Play(GroundSlamEndMontage);
			}
		}
		OnImpact();
		MyTimeline.Stop();
	}
	if (Value >= 1.0)
	{
		if (CharacterAnimInstance)
		{
			if (GroundSlamEndMontage)
			{
				CharacterAnimInstance->Montage_Play(GroundSlamEndMontage);
			}
		}
		OnImpact();
	}
}

void AGroundSlam::OnImpact()
{
	DamageSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TArray<AActor*> OverlappingActors;
	DamageSphere->GetOverlappingActors(OverlappingActors, TSubclassOf<AKingOfTheGunCharacter>());

	if (ImpactNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactNiagara, GetActorLocation());
	}
	if (SB_Impact)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SB_Impact, GetActorLocation());
	}

	for (AActor* Actor : OverlappingActors)
	{
		DamageWall(Actor, Actor->GetTargetLocation());
		if (AKingOfTheGunCharacter* KotgCharacter = Cast<AKingOfTheGunCharacter>(Actor))
		{
			if (KotgCharacter != Character)
			{
				TArray<AActor*> Ignored = TArray<AActor*>();
				Ignored.Add(Character);
				//Damage player hit
				UGameplayStatics::ApplyRadialDamage(Character, Damage, GetActorLocation(), DamageSphere->GetScaledSphereRadius(),
													UDamageType::StaticClass(), Ignored, this, (AController*)GetOwner(), true, ECC_Visibility);
				Character->PlayHitMarkAnimation();
			}		
		}
	}
	Destroy();
}

void AGroundSlam::MovePlayer(AKingOfTheGunCharacter* CharacterOwner, UAnimInstance* AnimInstance, FVector end)
{
	CharacterAnimInstance = AnimInstance;
	Character = CharacterOwner;
	Start = Character->GetActorLocation();
	End = end;
	MyTimeline.PlayFromStart();
	MyTimeline.SetPlayRate(FMath::Clamp(End.Size() / 600, 3.0f, 2.5f));
}
