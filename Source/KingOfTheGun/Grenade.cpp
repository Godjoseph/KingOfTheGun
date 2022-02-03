// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "KingOfTheGunCharacter.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GrenadeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = GrenadeMesh;

	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	FuseLength = 5.0f;
	Damage = 50.0f;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
	//Play throwing animation
	if (ThrowAnimation)
	{
		GrenadeMesh->PlayAnimation(ThrowAnimation, false);
		GrenadeMesh->SetOnlyOwnerSee(true);
		GrenadeMesh->SetOwnerNoSee(false);
	}
}

void AGrenade::OnRelease(AKingOfTheGunCharacter* CharacterOwner, FVector ForwardVector)
{
	GrenadeOwner = CharacterOwner;
	ForwardVector *= 2100.0f;
	ForwardVectorToApply = ForwardVector;

	GrenadeMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GrenadeMesh->SetOnlyOwnerSee(false);
	GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GrenadeMesh->SetSimulatePhysics(true);
	GrenadeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	GrenadeMesh->AddImpulse(ForwardVectorToApply);

	FTimerHandle GrenadeHandler;
	GetWorldTimerManager().SetTimer(GrenadeHandler, this, &AGrenade::ContinueOnRelease, 0.2, false);
}

void AGrenade::ContinueOnRelease()
{
	GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FTimerHandle GrenadeFuseHandler;
	GetWorldTimerManager().SetTimer(GrenadeFuseHandler, this, &AGrenade::Explode, FuseLength, false);
}

void AGrenade::Explode()
{
	FVector ExplosionLocation = GrenadeMesh->GetComponentLocation();
	DamageSphere->SetWorldLocation(ExplosionLocation);

	if (NS_Explosion)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Explosion, ExplosionLocation);
	}
	if (SB_Explosion)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Explosion, ExplosionLocation);
	}

	TArray<AActor*> OverlappingActors;
	DamageSphere->GetOverlappingActors(OverlappingActors, TSubclassOf<AKingOfTheGunCharacter>());

	for (AActor* Actor : OverlappingActors)
	{
		//DamageWall(Actor, Actor->GetTargetLocation());
		
		if (AKingOfTheGunCharacter* Character = Cast<AKingOfTheGunCharacter>(Actor))
		{
			GrenadeOwner->PlayHitMarkAnimation();
		}
	}
	//Damage player hit
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageSphere->GetScaledSphereRadius(),
										UDamageType::StaticClass(), TArray<AActor*>(), this, (AController*)GetOwner(), true, ECC_Visibility);

	Destroy();
}
