// Copyright Epic Games, Inc. All Rights Reserved.

#include "KingOfTheGunCharacter.h"
#include "KingOfTheGunProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Grenade.h"
#include "GroundSlam.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "AI/Bot_AIController.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AKingOfTheGunCharacter

AKingOfTheGunCharacter::AKingOfTheGunCharacter()
{
	// Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	PrimaryActorTick.bCanEverTick = true;

	// set our health and shield
	FullHealth = 100.f;
	Health = FullHealth;
	HealthPercentage = 1.0f;
	PreviousHealth = HealthPercentage;
	FullShield = 100.f;
	Shield = FullShield;
	ShieldPercentage = 1.0f;
	PreviousShield = ShieldPercentage;
	isShielded = false;
	canFire = true;
	canShield = true;
	canThrowGrenade = true;
	canBash = true;
	canGroundSlam = true;
	isDead = false;
	isThirdPerson = false;
	baseGun = true; 
	isSlaming = false;
	isThrowingGrenade = false;
	isBashing = false;
	ammoType = 0;
	gunType = 0;
	fromInteract = false;
	nbBulletShot = 0;
	grenadeAmount = 4;
	isRespawning = false;

	/*if (HealthCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("SetHealthValue"));
		TimelineFinishedCallback.BindUFunction(this, FName("SetHealthState"));
		MyTimeline.AddInterpFloat(HealthCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

	if (ShieldCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("SetShieldValue"));
		TimelineFinishedCallback.BindUFunction(this, FName("SetShieldState"));
		MyTimeline.AddInterpFloat(ShieldCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}*/

	// set our turn rates for input
	BaseTurnRate = 90.f;
	BaseLookUpRate = 90.f;

	//COMMENTÉE POUR REPOSITIONNER LA 1ST PERSON CAMERA AU 3RD PERSON CAMERA - Mike
	// Create a first person camera boom (pulls in towards the player if there is a collision)	
	//FirstPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("FirstPersonCameraBoom"));
	//FirstPersonCameraBoom->SetupAttachment(RootComponent);
	//FirstPersonCameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	//FirstPersonCameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a ThirdPersonCameraComponent
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCamera"));
	ThirdPersonCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	ThirdPersonCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	Mesh3P = GetMesh();

	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetOwnerNoSee(true);
	ShieldMesh->bCastDynamicShadow = false;
	ShieldMesh->CastShadow = false;
	ShieldMesh->SetupAttachment(RootComponent);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	// Create a gun mesh component Third Person
	TP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TP_Gun"));
	TP_Gun->SetOwnerNoSee(true);
	TP_Gun->bCastDynamicShadow = false;
	TP_Gun->CastShadow = false;
	TP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	TP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TPMuzzleLocation"));
	TP_MuzzleLocation->SetupAttachment(TP_Gun);
	TP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	interactRange = CreateDefaultSubobject<USphereComponent>(TEXT("InteractRange"));
	interactRange->SetupAttachment(RootComponent);
	interactRange->SetCollisionProfileName(TEXT("Pickup"));
	interactRange->SetSphereRadius(150.0f);

	BashDamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BashDamageBox"));
	BashDamageBox->SetupAttachment(Mesh3P, TEXT("RightHand"));

	TPCameraREF = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCameraREF"));
	TPCameraREF->SetupAttachment(RootComponent);
}

void AKingOfTheGunCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (ViewTransitionCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		ViewTransitionTimeline.AddInterpFloat(ViewTransitionCurve, TimelineProgress);
		ViewTransitionTimeline.SetLooping(false);
		FOnTimelineFloat TimelineProgressShield;
		TimelineProgressShield.BindUFunction(this, FName("TimelineProgressShield"));
		ShieldTimeline.AddInterpFloat(ViewTransitionCurve, TimelineProgressShield);
		ShieldTimeline.SetLooping(false);
	}

	if (equippedWeaponActor)
	{
		equippedWeapon = equippedWeaponActor.GetDefaultObject();
		baseWeaponActor = equippedWeaponActor;
		bullets = equippedWeapon->bullets;
		isLineTracing = equippedWeapon->isLineTracing;
		isPistol = equippedWeapon->isPistol;
		isShotgun = equippedWeapon->isShotgun;
		TimeBetweenShots = equippedWeapon->TimeBetweenShots;
		gunDamage = equippedWeapon->gunDamage;
		headshotDamage = equippedWeapon->headshotDamage;
		bulletSpread = equippedWeapon->bulletSpread;
		minimumRecoil = equippedWeapon->minimumRecoil;
		maximumRecoil = equippedWeapon->maximumRecoil;
		WeaponRange = equippedWeapon->WeaponRange;
	}

	if (equippedWeapon->SB_GunSound)
	{
		gunSound = equippedWeapon->SB_GunSound;
	}
	else
	{
		gunSound = FireSound;
	}

	baseGunMesh = equippedWeapon->mesh;

	myInstance = Cast<UKotgInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	USkeletalMeshComponent* ThirdPersonMesh = Cast<USkeletalMeshComponent>(GetDefaultSubobjectByName(TEXT("CharacterMesh0")));

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));
	TP_Gun->AttachToComponent(ThirdPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		Mesh1P->SetHiddenInGame(false, true);
	}

	ShieldMesh->SetVisibility(false);
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayerControllerRef = Cast<AKotgPlayerController>(GetController());
	ChangeCrosshair();
}

void AKingOfTheGunCharacter::OnGroundSlamOver()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bUseControllerRotationYaw = true;
}

void AKingOfTheGunCharacter::OnDyingOver()
{
	Mesh3P->SetVisibility(false);
	TP_Gun->SetVisibility(false);
}

void AKingOfTheGunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isDead && !isThirdPerson) {
		AimAssist(DeltaTime);
	}

	if (isSlaming)
	{
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		if (UAnimInstance* AnimInstance = Mesh3P->GetAnimInstance())
		{
			if (GroundSlamStartMontage)
			{
				AnimInstance->Montage_Play(GroundSlamStartMontage);
			}
		}
	}

	MyTimeline.TickTimeline(DeltaTime);
	ViewTransitionTimeline.TickTimeline(DeltaTime);
	ShieldTimeline.TickTimeline(DeltaTime);
}

///////////////////////////////////////////////////////////////////////////
// Input

void AKingOfTheGunCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AKingOfTheGunCharacter::JumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AKingOfTheGunCharacter::JumpEnd);

	// Bind groundslam events
	PlayerInputComponent->BindAction("GroundSlam", IE_Pressed, this, &AKingOfTheGunCharacter::GroundSlamStart);
	PlayerInputComponent->BindAction("GroundSlam", IE_Released, this, &AKingOfTheGunCharacter::GroundSlamEnd);

	// Bind bash even
	PlayerInputComponent->BindAction("Bash", IE_Pressed, this, &AKingOfTheGunCharacter::Bash);

	// Bind grenade event
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AKingOfTheGunCharacter::ThrowGrenade);

	// Bind crouch events
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AKingOfTheGunCharacter::CrouchStart);

	// Bind shield events
	PlayerInputComponent->BindAction("Shield", IE_Pressed, this, &AKingOfTheGunCharacter::ShieldStart);
	PlayerInputComponent->BindAction("Shield", IE_Released, this, &AKingOfTheGunCharacter::ShieldEnd);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AKingOfTheGunCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AKingOfTheGunCharacter::StopFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AKingOfTheGunCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKingOfTheGunCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AKingOfTheGunCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &AKingOfTheGunCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AKingOfTheGunCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AKingOfTheGunCharacter::LookUpAtRate);

	InputComponent->BindAction("WeaponPickUp", IE_Pressed, this, &AKingOfTheGunCharacter::Interact);
	InputComponent->BindAction("WeaponToss", IE_Pressed, this, &AKingOfTheGunCharacter::WeaponToss);

	PlayerInputComponent->BindAction("FreeLookCamera", IE_Pressed, this, &AKingOfTheGunCharacter::ActivateFreeLookCamera);
	PlayerInputComponent->BindAction("FreeLookCamera", IE_Released, this, &AKingOfTheGunCharacter::DeactivateFreeLookCamera);
}

void AKingOfTheGunCharacter::ThrowGrenade()
{
	if (grenadeAmount > 0 && canThrowGrenade && !isDead)
	{
		StopFire();
		canShield = false;
		canFire = false;
		canBash = false;
		canGroundSlam = false;
		canThrowGrenade = false;
		isThrowingGrenade = true;
		if (GrenadeClass)
		{
			Grenade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass);
			Grenade->SetOwner(this);
			if (Grenade)
			{
				FP_Gun->SetVisibility(false);
				TP_Gun->SetVisibility(false);
				Grenade->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_r"));
				if (UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance())
				{
					if (GrenadeMontage)
					{
						AnimInstance->Montage_Play(GrenadeMontage);
					}
				}
				if (UAnimInstance* AnimInstance = Mesh3P->GetAnimInstance())
				{
					if (GrenadeMontage3P)
					{
						AnimInstance->Montage_Play(GrenadeMontage3P);
					}
				}
			}
			grenadeAmount--;
		}
	}
}


void AKingOfTheGunCharacter::OnGrenadeReleased()
{
	if (Grenade)
	{
		isThrowingGrenade = false;
		Grenade->OnRelease(this, UKismetMathLibrary::GetForwardVector(GetControlRotation()));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefireClick, this, &AKingOfTheGunCharacter::ResetFire, 0.2, false);
		FP_Gun->SetVisibility(true);
		TP_Gun->SetVisibility(true);
	}
}

void AKingOfTheGunCharacter::Bash()
{
	if (!isDead && canBash)
	{
		StopFire();
		canShield = false;
		canFire = false;
		canBash = false;
		canGroundSlam = false;
		canThrowGrenade = false;
		isBashing = true;
		if (UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance())
		{
			if (BashMontage)
			{
				AnimInstance->Montage_Play(BashMontage);
			}
		}
		if (UAnimInstance* AnimInstance = Mesh3P->GetAnimInstance())
		{
			if (BashMontage3P)
			{
				AnimInstance->Montage_Play(BashMontage3P);
			}
		}
	}
}

void AKingOfTheGunCharacter::OnBashDamage()
{
	TArray<AActor*> OverlappingActors;
	BashDamageBox->GetOverlappingActors(OverlappingActors, TSubclassOf<AKingOfTheGunCharacter>());

	for (AActor* Actor : OverlappingActors)
	{
		DamageWall(Actor, Actor->GetTargetLocation());
		if (AKingOfTheGunCharacter* Character = Cast<AKingOfTheGunCharacter>(Actor))
		{
			if (Character != this)
			{
				//Damage player hit
				UGameplayStatics::ApplyDamage(Character, 25, nullptr, this, UDamageType::StaticClass());
				PlayHitMarkAnimation();
			}
		}
	}
	if (SB_Bash)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SB_Bash, GetActorLocation());
	}
	isBashing = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefireClick, this, &AKingOfTheGunCharacter::ResetFire, 0.2, false);
}

void AKingOfTheGunCharacter::JumpStart()
{
	if (!isDead)
	{
		Jump();
	}
}

void AKingOfTheGunCharacter::JumpEnd() 
{
	if (!isDead)
	{
		StopJumping();
	}
}

void AKingOfTheGunCharacter::GroundSlamStart()
{
	if (GetCharacterMovement()->IsFalling() && canGroundSlam && !isDead)
	{
		if (GroundSlamClass)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HandleRefireClick);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HandleDeactivateThirdPerson);
			isSlaming = true;
			canShield = false;
			canFire = false;
			canBash = false;
			canThrowGrenade = false;
			ActivateThirdPerson();
			FRotator OldRotation = GetController()->GetControlRotation();
			GetController()->SetControlRotation(FRotator(-40, OldRotation.Yaw, OldRotation.Roll));
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefireClick, this, &AKingOfTheGunCharacter::ResetFire, 3, false);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleDeactivateThirdPerson, this, &AKingOfTheGunCharacter::DeactivateThirdPerson, 3, false);

			if (SB_GroundSlamCharge)
			{
				GroundSlamAudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SB_GroundSlamCharge, GetActorLocation());
			}
		}
	}
}

void AKingOfTheGunCharacter::GroundSlamEnd()
{
	if (GetCharacterMovement()->IsFalling() && isSlaming && !isDead)
	{
		GroundSlamAudioComponent->Stop();
		isSlaming = false;
		FHitResult Hit;
		const FVector StartTrace = ThirdPersonCameraComponent->GetComponentLocation();
		FVector EndTrace = (ThirdPersonCameraComponent->GetForwardVector() * WeaponRange) + StartTrace;
		FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
		{
			canGroundSlam = false;
			GroundSlam = GetWorld()->SpawnActor<AGroundSlam>(GroundSlamClass);
			GroundSlam->AttachToComponent(Mesh3P, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_l"));
			GroundSlam->MovePlayer(this, Mesh3P->GetAnimInstance(), Hit.Location);
			GetCharacterMovement()->DisableMovement();
		}
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefireClick, this, &AKingOfTheGunCharacter::ResetFire, 1.5, false);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleDeactivateThirdPerson, this, &AKingOfTheGunCharacter::DeactivateThirdPerson, 1.5, false);
	}
}

void AKingOfTheGunCharacter::Interact()
{
	if (!isDead)
	{
		TArray<AActor*> items;
		interactRange->GetOverlappingActors(items);

		for (int i = 0; i < items.Num(); i++)
		{
			AWeaponPickup* const weapon = Cast<AWeaponPickup>(items[i]);
			if (weapon && weapon->active)
			{
				fromInteract = true;

				WeaponToss();

				equippedWeaponActor = weapon->GetClass();

				FP_Gun->SetSkeletalMesh(weapon->mesh->SkeletalMesh);
				TP_Gun->SetSkeletalMesh(weapon->mesh->SkeletalMesh);
				baseGun = false;
				bullets = weapon->bullets;
				isLineTracing = weapon->isLineTracing;
				isPistol = weapon->isPistol;
				isShotgun = weapon->isShotgun;
				TimeBetweenShots = weapon->TimeBetweenShots;
				gunDamage = weapon->gunDamage;
				bulletSpread = weapon->bulletSpread;
				minimumRecoil = weapon->minimumRecoil;
				maximumRecoil = weapon->maximumRecoil;
				WeaponRange = weapon->WeaponRange;
				ammoType = weapon->ammoType;
				gunType = weapon->gunType;
				ChangeCrosshair();
				if (weapon->SB_GunSound)
				{
					gunSound = weapon->SB_GunSound;
				}
				else
				{
					gunSound = FireSound;
				}
				if (SB_Pickup)
				{
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SB_Pickup, GetActorLocation());
				}

				ChangeProjectile();
				weapon->Destroy();
				fromInteract = false;
				break;
			}
		}
	}
}

void AKingOfTheGunCharacter::WeaponToss()
{
	if (!isDead)
	{
		if (baseGun) return;

		FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
		auto Thrown = Cast<AWeaponPickup>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), equippedWeaponActor, SpawnTransform));
		if (Thrown)
		{
			Thrown->Init();
			UGameplayStatics::FinishSpawningActor(Thrown, SpawnTransform);
		}
		/*Thrown->mesh->SetSkeletalMesh(equippedWeapon->mesh->SkeletalMesh);

		Thrown->ammoType = ammoType;
		Thrown->gunType = gunType;
		Thrown->isLineTracing = isLineTracing;
		Thrown->isShotgun = isShotgun;
		Thrown->TimeBetweenShots = TimeBetweenShots;
		Thrown->gunDamage = gunDamage;
		Thrown->bulletSpread = bulletSpread;
		Thrown->minimumRecoil = minimumRecoil;
		Thrown->maximumRecoil = maximumRecoil;
		Thrown->WeaponRange = WeaponRange;*/
		Thrown->bullets = bullets;

		if (bullets <= 0)
		{
			Thrown->SetLifeSpan(3.0f);
			Thrown->active = false;
		}
		else {
			Thrown->startHover();
		}

		if (!fromInteract)
		{
			equippedWeapon = baseWeaponActor.GetDefaultObject();
			FP_Gun->SetSkeletalMesh(baseGunMesh->SkeletalMesh);
			TP_Gun->SetSkeletalMesh(baseGunMesh->SkeletalMesh);
			baseGun = true;
		}

		if (SB_Drop)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SB_Drop, GetActorLocation());
		}

		bullets = equippedWeapon->bullets;
		isLineTracing = equippedWeapon->isLineTracing;
		isPistol = equippedWeapon->isPistol;
		isShotgun = equippedWeapon->isShotgun;
		TimeBetweenShots = equippedWeapon->TimeBetweenShots;
		gunDamage = equippedWeapon->gunDamage;
		if (equippedWeapon->SB_GunSound)
		{
			gunSound = equippedWeapon->SB_GunSound;
		}
		else
		{
			gunSound = FireSound;
		}
		bulletSpread = equippedWeapon->bulletSpread;
		minimumRecoil = equippedWeapon->minimumRecoil;
		maximumRecoil = equippedWeapon->maximumRecoil;
		WeaponRange = equippedWeapon->WeaponRange;

		ChangeCrosshair();
	}
}

void AKingOfTheGunCharacter::OnFire()
{
	if (canFire && !isDead)
	{
		// try and fire a projectile
		if (ProjectileClass != NULL)
		{
			if (!baseGun)
			{
				if (bullets <= 0)return;
				else bullets--;
			}

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				if (bUsingMotionControllers)
				{
					const FRotator SpawnRotation = TP_MuzzleLocation->GetComponentRotation();
					const FVector SpawnLocation = TP_MuzzleLocation->GetComponentLocation();
					World->SpawnActor<AKingOfTheGunProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
				}
				else
				{
					const FRotator SpawnRotation = GetControlRotation();
					// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
					const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					// spawn the projectile at the muzzle
					FTransform SpawnTransform(SpawnRotation, SpawnLocation);
					AKingOfTheGunProjectile* bullet = Cast<AKingOfTheGunProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform));
					if (bullet != nullptr)
					{
						bullet->SetInstigator(GetController()->GetPawn());
						UGameplayStatics::FinishSpawningActor(bullet, SpawnTransform);
					}
				}
			}
		}
		// try and play the sound if specified
		if (gunSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, gunSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefireClick, this, &AKingOfTheGunCharacter::ResetFire, TimeBetweenShots, false);
	}
}

void AKingOfTheGunCharacter::StartFire()
{
	if (canFire && !isDead && bullets > 0)
	{
		canShield = false;
		canFire = false;
		canBash = false;
		canGroundSlam = false;
		canThrowGrenade = false;
		if (isLineTracing)
		{
			FireShot();
			if (!isPistol)
			{
				GetWorldTimerManager().SetTimer(TimerHandle_HandleRefire, this, &AKingOfTheGunCharacter::FireShot, TimeBetweenShots, true);
			}
		}
		else
		{
			// try and fire a projectile
			if (ProjectileClass != NULL)
			{
				if (!baseGun)
				{
					if (bullets <= 0)return;
					else bullets--;
				}

				UWorld* const World = GetWorld();
				if (World != NULL)
				{
					if (bUsingMotionControllers)
					{
						const FRotator SpawnRotation = TP_MuzzleLocation->GetComponentRotation();
						const FVector SpawnLocation = TP_MuzzleLocation->GetComponentLocation();
						World->SpawnActor<AKingOfTheGunProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
					}
					else
					{
						const FRotator SpawnRotation = GetControlRotation();
						// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
						const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						// spawn the projectile at the muzzle
						FTransform SpawnTransform(SpawnRotation, SpawnLocation);
						AKingOfTheGunProjectile* bullet = Cast<AKingOfTheGunProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform));
						if (bullet != nullptr)
						{
							bullet->SetInstigator(GetController()->GetPawn());
							UGameplayStatics::FinishSpawningActor(bullet, SpawnTransform);
						}
					}
				}
			}
			// try and play the sound if specified
			if (gunSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, gunSound, GetActorLocation());
			}

			// try and play a firing animation if specified
			if (FireAnimation != NULL)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
				if (AnimInstance != NULL)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}
		}
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefireClick, this, &AKingOfTheGunCharacter::ResetAction, 0.2, false);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleRefireClick, this, &AKingOfTheGunCharacter::ResetFire, TimeBetweenShots, false);
	}
}

void AKingOfTheGunCharacter::StopFire()
{
	if (!isShielded && !isSlaming && !isBashing && !isThrowingGrenade)
	{
		bulletSpread = 0;
		nbBulletShot = 0;
		GetWorldTimerManager().ClearTimer(TimerHandle_HandleRefire);
		ResetAction();
	}
}

void AKingOfTheGunCharacter::ResetFire()
{
	if (!isShielded && !isBashing && !isThrowingGrenade && !isDead)
	{
		canFire = true;
		ResetAction();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HandleRefireClick);
	}
}

void AKingOfTheGunCharacter::FireShot()
{
	if (bullets > 0)
	{
		bullets--;
		float maxBulletSpread = 600.f;
		if (nbBulletShot >= 2 && nbBulletShot < 10)
		{
			bulletSpread += (maxBulletSpread) / 10;
		}
		if (nbBulletShot == 0 && GetVelocity().Size() > 0)
		{
			bulletSpread += 400;
		}

		if (MuzzleParticles)
		{
			UParticleSystemComponent* FPmuzzleParticle = UGameplayStatics::SpawnEmitterAttached(MuzzleParticles, FP_Gun, TEXT("Muzzle"));
			FPmuzzleParticle->SetOnlyOwnerSee(true);
			FPmuzzleParticle->SetOwnerNoSee(false);
			UParticleSystemComponent* TPmuzzleParticle = UGameplayStatics::SpawnEmitterAttached(MuzzleParticles, TP_Gun, TEXT("MuzzleTP"));
			TPmuzzleParticle->SetOnlyOwnerSee(false);
			TPmuzzleParticle->SetOwnerNoSee(true);
		}

		if (isShotgun)
		{
			ProduceBullet(-200, 350, 350);
			ProduceBullet(200, -350, -350);
			ProduceBullet(-350, 200, 200);
			ProduceBullet(350, -200, -200);
			ProduceBullet(-275, 275, 275);
			ProduceBullet(275, -275, -275);
		}
		else
		{
			ProduceBullet(0, 0, 0);
		}


		nbBulletShot++;
		float recoil = FMath::RandRange(minimumRecoil, maximumRecoil);
		AddControllerPitchInput(-recoil);

		// try and play the sound if specified
		if (gunSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, gunSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

void AKingOfTheGunCharacter::ProduceBullet(float offsetX, float offsetY, float offsetZ)
{
	FHitResult Hit;
	UNiagaraComponent* FPNiagara;
	UNiagaraComponent* TPNiagara;
	float randomFloatX = FMath::RandRange(-bulletSpread, bulletSpread);
	float randomFloatY = FMath::RandRange(-bulletSpread, bulletSpread);
	float randomFloatZ = FMath::RandRange(-bulletSpread, bulletSpread);
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndTrace = (FirstPersonCameraComponent->GetForwardVector() * WeaponRange) + StartTrace;	
	EndTrace.X += randomFloatX + offsetX;
	EndTrace.Y += randomFloatY + offsetY;
	EndTrace.Z += randomFloatZ + offsetZ;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);

	if (SystemTemplateNiagara)
	{
		FPNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(SystemTemplateNiagara, FP_Gun, TEXT("MuzzleTP"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::None, true);
		FPNiagara->SetVectorParameter("BeamEnd", EndTrace);
		FPNiagara->SetOwnerNoSee(false);
		FPNiagara->SetOnlyOwnerSee(true);
		FPNiagara->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		TPNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(SystemTemplateNiagara, TP_Gun, TEXT("MuzzleTP"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::None, true);
		TPNiagara->SetVectorParameter("BeamEnd", EndTrace);
		TPNiagara->SetOwnerNoSee(true);
		TPNiagara->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
		{
			FPNiagara->SetVectorParameter("BeamEnd", Hit.ImpactPoint);
			TPNiagara->SetVectorParameter("BeamEnd", Hit.ImpactPoint);
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint));
			}
			if (Hit.GetComponent()->GetName() == FString("ExplosiveBarrel"))
			{
				UGameplayStatics::ApplyDamage(Hit.GetActor(), gunDamage, PlayerControllerRef, this, UDamageType::StaticClass());
			}
			else if (Hit.GetComponent()->GetName() == FString("ShieldMesh"))
			{
				UGameplayStatics::ApplyDamage(Hit.GetActor(), -gunDamage, PlayerControllerRef, this, UDamageType::StaticClass());
			}
			else if (Hit.GetActor())
			{
				DamageWall(Hit.GetActor(), Hit.ImpactPoint);
				if (Hit.GetComponent()->GetName() == FString("CharacterMesh0"))
				{
					if (Hit.BoneName == "Head"){
						UGameplayStatics::ApplyDamage(Hit.GetActor(), headshotDamage, PlayerControllerRef, this, UDamageType::StaticClass());
					}
					else {
						UGameplayStatics::ApplyDamage(Hit.GetActor(), gunDamage, PlayerControllerRef, this, UDamageType::StaticClass());
					}
					PlayHitMarkAnimation();
				}
			}
		}
	}
}

void AKingOfTheGunCharacter::ResetAction()
{
	canShield = true;
	canThrowGrenade = true;
	canBash = true;
	canGroundSlam = true;
	isSlaming = false;
}

void AKingOfTheGunCharacter::Respawn()
{
	bUseControllerRotationYaw = true;
	isRespawning = false;
	isDead = false;

	equippedWeapon = baseWeaponActor.GetDefaultObject();
	FP_Gun->SetSkeletalMesh(baseGunMesh->SkeletalMesh);
	TP_Gun->SetSkeletalMesh(baseGunMesh->SkeletalMesh);
	baseGun = true;

	bullets = equippedWeapon->bullets;
	isLineTracing = equippedWeapon->isLineTracing;
	isPistol = equippedWeapon->isPistol;
	isShotgun = equippedWeapon->isShotgun;
	TimeBetweenShots = equippedWeapon->TimeBetweenShots;
	gunDamage = equippedWeapon->gunDamage;
	bulletSpread = equippedWeapon->bulletSpread;
	minimumRecoil = equippedWeapon->minimumRecoil;
	maximumRecoil = equippedWeapon->maximumRecoil;
	WeaponRange = equippedWeapon->WeaponRange;
	grenadeAmount = 4;

	ChangeCrosshair();

	HealthPercentage = 1.0;
	ShieldPercentage = 1.0;
	Health = 100;
	Shield = 100;
	Mesh1P->SetOwnerNoSee(true);
	DeactivateThirdPerson();
	ShieldMesh->SetOwnerNoSee(false);
	TP_Gun->SetOwnerNoSee(true);
	FP_Gun->SetOwnerNoSee(false);
}

bool AKingOfTheGunCharacter::GetIsDead()
{
	return isDead;
}

void AKingOfTheGunCharacter::InvertControls()
{
	Sensitivity = -Sensitivity;
}

void AKingOfTheGunCharacter::SetSensitivity(float sensitivity)
{
	Sensitivity = sensitivity;
}

void AKingOfTheGunCharacter::SetAimAssist(bool aimAssist)
{
	isAimAssistActivated = aimAssist;
}

bool AKingOfTheGunCharacter::GetIsShielded()
{
	return isShielded;
}

float AKingOfTheGunCharacter::GetTurnValue()
{
	return turnValue;
}

float AKingOfTheGunCharacter::GetTurnAtValue()
{
	return turnAtValue;
}

void AKingOfTheGunCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && !isDead && !isSlaming)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AKingOfTheGunCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !isDead && !isSlaming)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AKingOfTheGunCharacter::CrouchStart()
{
	if (!isDead)
	{
		if (GetCharacterMovement()->bWantsToCrouch)
		{
			PlayerUncrouched();
			ClientCrouch(false);
		}
		else
		{
			PlayerCrouched();
			ClientCrouch(true);
		}
	}
}

void AKingOfTheGunCharacter::CrouchEnd()
{
	if (!isDead)
	{
		PlayerUncrouched();
		ClientCrouch(false);
	}
}

void AKingOfTheGunCharacter::ClientCrouch(bool Value) 
{
	if (Value)
	{
		GetCharacterMovement()->bWantsToCrouch = true;
	}
	else
	{
		GetCharacterMovement()->bWantsToCrouch = false;
	}
}

void AKingOfTheGunCharacter::TimelineProgress(float Value)
{
	float NewArmLength = FMath::Lerp(1, 2, Value);
	CameraBoom->TargetArmLength = NewArmLength;
	
	if (Value <= 50 && !isThirdPerson)
	{
		FirstPersonCameraComponent->SetActive(true);
		ThirdPersonCameraComponent->SetActive(false);
		FP_Gun->SetVisibility(true);
		TP_Gun->SetOwnerNoSee(true);
		Mesh3P->SetOwnerNoSee(true);
		Mesh1P->SetOwnerNoSee(false);
		ResetFire();
	}
}

void AKingOfTheGunCharacter::TimelineProgressShield(float Value)
{
	if (isShielded)
	{
		float NewOpacity = FMath::Lerp(0.0, 0.5, Value / 600);
		ShieldMesh->SetScalarParameterValueOnMaterials(TEXT("Opacity"), NewOpacity);
	}
	if (!isShielded)
	{
		float NewOpacity = FMath::Lerp(0.0, 0.5, Value / 600);
		if (Value <= 50)
		{
			NewOpacity = 0.0;
		}
		ShieldMesh->SetScalarParameterValueOnMaterials(TEXT("Opacity"), NewOpacity);
	}
}

void AKingOfTheGunCharacter::ShieldStart()
{
	if (ShieldPercentage > 0 && !isDead && canShield)
	{
		if (SB_ShieldUp)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SB_ShieldUp, GetActorLocation());
		}
		GetWorldTimerManager().ClearTimer(TimerHandle_HandleRefire);
		GetCharacterMovement()->MaxWalkSpeed = 220;
		isShielded = true;
		ActivateThirdPerson();
		ShieldTimeline.ReverseFromEnd();
		canThrowGrenade = false;
		canBash = false;
		canGroundSlam = false;
		canFire = false;
		ShieldMesh->SetVisibility(true);
		ShieldMesh->SetOwnerNoSee(false);
		ShieldMesh->SetCollisionProfileName(TEXT("Shield"));
	}
}

void AKingOfTheGunCharacter::ActivateThirdPerson()
{
	isThirdPerson = true;
	Mesh3P->SetOwnerNoSee(false);
	Mesh1P->SetOwnerNoSee(true);
	FP_Gun->SetVisibility(false);
	TP_Gun->SetOwnerNoSee(false);
	FirstPersonCameraComponent->SetActive(false);
	ThirdPersonCameraComponent->SetActive(true);
	ViewTransitionTimeline.ReverseFromEnd();
}

void AKingOfTheGunCharacter::ShieldEnd()
{
	if (!isDead && canShield)
	{
		if (SB_ShieldDown)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SB_ShieldDown, GetActorLocation());
		}
		GetCharacterMovement()->MaxWalkSpeed = 600;
		isShielded = false;
		DeactivateThirdPerson();
		ShieldTimeline.PlayFromStart();
		bUseControllerRotationYaw = true;
		ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AKingOfTheGunCharacter::DeactivateThirdPerson()
{
	if (!isDead)
	{
		isThirdPerson = false;
		ViewTransitionTimeline.PlayFromStart();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HandleDeactivateThirdPerson);
	}
}

void AKingOfTheGunCharacter::ActivateFreeLookCamera()
{
	if (isShielded)
	{
		bUseControllerRotationYaw = false;
	}
}

void AKingOfTheGunCharacter::DeactivateFreeLookCamera()
{
	if (isShielded)
	{
		bUseControllerRotationYaw = true;
	}
}

void AKingOfTheGunCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * Sensitivity * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	turnAtValue = GetInputAxisValue("TurnRate");
}

void AKingOfTheGunCharacter::Turn(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * Sensitivity * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	turnValue = GetInputAxisValue("Turn");
}

void AKingOfTheGunCharacter::LookUpAtRate(float Rate)
{
	if (isSlaming)
	{
		//ThirdPersonCameraComponent->bUsePawnControlRotation = true;
		if (GetController()->GetControlRotation().Pitch <= 320)
		{
			// calculate delta for this frame from the rate information
			AddControllerPitchInput(Rate * Sensitivity * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		}
		else if (GetController()->GetControlRotation().Pitch > 320)
		{
			// calculate delta for this frame from the rate information
			FRotator OldRotation = GetController()->GetControlRotation();
			GetController()->SetControlRotation(FRotator(-40, OldRotation.Yaw, OldRotation.Roll));
		}
		ThirdPersonCameraComponent->bUsePawnControlRotation = true;
	}
	else
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * Sensitivity * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AKingOfTheGunCharacter::AimAssist(float DeltaTime)
{
	myInstance = Cast<UKotgInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//si le joueur joue au clavier, desactive
	if ((myInstance->GetFirstLocalPlayerController() == this->GetController()) && myInstance->GetOffset()) {
		return;
	}
	//si l'option est desactive
	if (!isAimAssistActivated) {
		return;
	}
	/*
	if (FMath::Abs(GetInputAxisValue("Turn")) > 0.05f || FMath::Abs(GetInputAxisValue("TurnRate")) > 0.05f) {
		return;
	}*/

	//on trouve tous les joueurs
	TArray<AActor*> allPlayers;
	if (allPlayers.Num() <= 0) {
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKingOfTheGunCharacter::StaticClass(), allPlayers);
	}

	//vecteur pour la camera
	FVector MyCameraForwardVector = FirstPersonCameraComponent->GetForwardVector().GetSafeNormal();
	FVector MyCameraRightVector = FirstPersonCameraComponent->GetRightVector().GetSafeNormal();
	FVector MyCameraUpVector = FirstPersonCameraComponent->GetUpVector().GetSafeNormal();
	

	//on trouve la meilleure cible
	FVector VectorToTarget;
	AActor* bestTarget = this;
	float bestDotProduct = 0.f;

	for (AActor* myTarget : allPlayers) {
		if (GetController()) {
			if (!(myTarget == this) && GetController()->LineOfSightTo(myTarget) && !(Cast<AKingOfTheGunCharacter>(myTarget)->isDead) && (GetDistanceTo(myTarget) < 3000.f)) {
				//check si la tete est la meilleure cible
				FVector AimAssistedViewDirection = (myTarget->GetActorLocation() - FirstPersonCameraComponent->GetComponentLocation() + FVector(0.f, 0.f, 80.f)).GetSafeNormal();
				float dotProductForward = FVector::DotProduct(AimAssistedViewDirection, MyCameraForwardVector);
				if (dotProductForward > bestDotProduct) {
					bestDotProduct = dotProductForward;
					VectorToTarget = AimAssistedViewDirection;
					bestTarget = myTarget;
				}
				//check si le corps est la meilleure cible
				AimAssistedViewDirection = (myTarget->GetActorLocation() - FirstPersonCameraComponent->GetComponentLocation()).GetSafeNormal();
				dotProductForward = FVector::DotProduct(AimAssistedViewDirection, MyCameraForwardVector);
				if (dotProductForward > bestDotProduct) {
					bestDotProduct = dotProductForward;
					VectorToTarget = AimAssistedViewDirection;
					bestTarget = myTarget;
				}
			}
		}
	}
	

	//on ajuste la camera

	const float minValue = 0.996f;
	if (bestDotProduct > minValue) {
		float ajustementHorizontal = FVector::DotProduct(VectorToTarget, MyCameraRightVector) * (bestDotProduct - minValue) / (1 - minValue);
		float ajustementVertical = -FVector::DotProduct(VectorToTarget, MyCameraUpVector) * (bestDotProduct - minValue) / (1 - minValue);

		if (ajustementHorizontal > 0.02f) {
			ajustementHorizontal = (bestDotProduct - minValue) / (1 - minValue) * (FMath::Min(ajustementHorizontal, 0.1f) / 0.15f) * (1/(DeltaTime * 60.f)) / Sensitivity;
		}
		else if (ajustementHorizontal < 0.02f) {
			ajustementHorizontal = (bestDotProduct - minValue) / (1 - minValue) * (FMath::Max(ajustementHorizontal, -0.1f) / 0.15f) * (1 / (DeltaTime * 60.f)) / Sensitivity;
		}
		if (ajustementVertical > 0.02f) {
			ajustementVertical = (bestDotProduct - minValue) / (1 - minValue) * (FMath::Min(ajustementVertical, 0.1f) / 0.15f) * (1 / (DeltaTime * 60.f)) / Sensitivity;
		}
		else if (ajustementVertical < 0.02f) {
			ajustementVertical = (bestDotProduct - minValue) / (1 - minValue) * (FMath::Max(ajustementVertical, -0.1f) / 0.15f) * (1 / (DeltaTime * 60.f)) / Sensitivity;
		}
		//lookUpValue_ = lookUpValue_ - 66.7f / this->GetDistanceTo(bestTarget) * (bestDotProduct - minValue) / (1 - minValue); //pour viser la tete
		Turn(ajustementHorizontal);
		LookUpAtRate(ajustementVertical);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%lld"), GetController()->LineOfSightTo(myTarget)));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Distance: %f"), this->GetDistanceTo(bestTarget)));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Dot : %f"), bestDotProduct));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Right : %f"), FVector::DotProduct(VectorToTarget, MyCameraRightVector)));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Up : %f"), FVector::DotProduct(VectorToTarget, MyCameraUpVector)));
}

float AKingOfTheGunCharacter::GetWeaponBullet()
{
	return bullets;
}

float AKingOfTheGunCharacter::GetGrenadeAmount()
{
	return grenadeAmount;
}

float AKingOfTheGunCharacter::GetHealth()
{
	return HealthPercentage;
}

FText AKingOfTheGunCharacter::GetHealthIntText()
{
	int32 HP = FMath::RoundHalfFromZero(HealthPercentage * 100);
	FString HPS = FString::FromInt(HP);
	FText HPTEXT = FText::FromString(HPS);
	return HPTEXT;
}

float AKingOfTheGunCharacter::GetShield()
{
	return ShieldPercentage;
}

FText AKingOfTheGunCharacter::GetShieldIntText()
{
	int32 SP = FMath::RoundHalfFromZero(ShieldPercentage * 100);
	FString SPS = FString::FromInt(SP);
	FText SPTEXT = FText::FromString(SPS);
	return SPTEXT;
}

void AKingOfTheGunCharacter::SetHealthValue()
{
	/*TimelineValue = MyTimeline.GetPlaybackPosition();
	CurveFloatValue = PreviousHealth + damageTaken * HealthCurve->GetFloatValue(TimelineValue);
	Health = CurveFloatValue * FullHealth;
	Health = FMath::Clamp(Health, 0.0f, FullHealth);
	HealthPercentage = CurveFloatValue;
	HealthPercentage = FMath::Clamp(Health, 0.0f, 1.0f);*/
}

void AKingOfTheGunCharacter::SetHealthState()
{
}

void AKingOfTheGunCharacter::SetHealthChange(float HealthValue)
{
	/*PreviousHealth = HealthPercentage;
	MyTimeline.PlayFromStart();*/
}

void AKingOfTheGunCharacter::SetShieldValue()
{
	/*TimelineValue = MyTimeline.GetPlaybackPosition();
	CurveFloatValue = PreviousShield + damageTaken * ShieldCurve->GetFloatValue(TimelineValue);
	Shield = CurveFloatValue * FullShield;
	Shield = FMath::Clamp(Shield, 0.0f, FullShield);
	ShieldPercentage = CurveFloatValue;
	ShieldPercentage = FMath::Clamp(Shield, 0.0f, 1.0f);*/
}

void AKingOfTheGunCharacter::SetShieldState()
{
}

void AKingOfTheGunCharacter::SetShieldChange(float ShieldValue)
{
	PreviousShield = ShieldPercentage;
	MyTimeline.PlayFromStart();
}

void AKingOfTheGunCharacter::UpdateShield(float ShieldValue)
{
	Shield += ShieldValue;
	Shield = FMath::Clamp(Shield, 0.0f, FullShield);
	PreviousShield = ShieldPercentage;
	ShieldPercentage = Shield / FullShield;
	if (ShieldPercentage <= 0)
	{
		ShieldEnd();
	}
}

bool AKingOfTheGunCharacter::DamageEffectAnim()
{
	if (DamageEffect)
	{
		DamageEffect = false;
		return true;
	}

	return false;
}

float AKingOfTheGunCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	//Cette fonction est nécessaire. Le take damage de Super fonctionne avec RadialDamageWithFalloff, mais pas celui de actor --Mike
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (isShielded && ActualDamage < 0)
	{
		UpdateShield(ActualDamage);
	}
	else if (Health > 0)
	{
		if (SB_DamageTaken)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SB_DamageTaken, GetActorLocation());
		}
		DamageEffect = true;
		UpdateHealth(-ActualDamage);
	}

	if (Cast<ABot_AIController>(GetController())) {
		ABot_AIController* AICtrler = Cast<ABot_AIController>(GetController());
		AICtrler->threat += ActualDamage;
	}

	return ActualDamage;
}

void AKingOfTheGunCharacter::UpdateHealth(float HealthValue)
{
	Health += HealthValue;
	Health = FMath::Clamp(Health, 0.0f, FullHealth);
	PreviousHealth = HealthPercentage;
	HealthPercentage = Health / FullHealth;

	if (Health <= 0)
	{
		if (!isDead)
		{
			nbLives--;
		}
		if (nbLives <= 0)
		{
			Mesh3P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		WeaponToss();
		isThirdPerson = false;
		bUseControllerRotationYaw = false;
		GetMesh()->SetOwnerNoSee(false);
		TP_Gun->SetOwnerNoSee(false);
		Mesh1P->SetOwnerNoSee(true);
		FP_Gun->SetOwnerNoSee(true);
		isDead = true;
		ActivateThirdPerson();
		isShielded = false;
		ShieldMesh->SetVisibility(false);
		ShieldMesh->SetOwnerNoSee(true);
		ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

FRotator AKingOfTheGunCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

bool AKingOfTheGunCharacter::GetIsShotgun()
{
	return isShotgun;
}

void AKingOfTheGunCharacter::ChangeProjectile_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("ChangeProjectile from blueprint"));
}

int AKingOfTheGunCharacter::GetNbBullets() 
{
	return bullets;
}