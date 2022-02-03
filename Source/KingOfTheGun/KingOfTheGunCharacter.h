// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
#include "KotgPlayerController.h"
#include "NiagaraSystem.h"
#include "KotgInstance.h"
#include "WeaponPickup.h"
#include "KingOfTheGunCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AKingOfTheGunCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) *///
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Shield mesh */
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class USkeletalMeshComponent* ShieldMesh;

	/** Shield mesh FP */
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class USkeletalMeshComponent* FPShieldMesh;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* TP_MuzzleLocation;

	/** Camera boom positioning the camera behind the character First person */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* FirstPersonCameraBoom;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TPCameraREF;

public:
	AKingOfTheGunCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseTurnRate;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class USkeletalMeshComponent* TP_Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int nbLives;

	bool isRespawning;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Projectile)
	TSubclassOf<class AKingOfTheGunProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UParticleSystem* MuzzleParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UNiagaraSystem* SystemTemplateNiagara;

	UPROPERTY(BlueprintReadOnly, Category = Gameplay)
	int nbBulletShot;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	TSubclassOf<AWeaponPickup> equippedWeaponActor;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	TSubclassOf<AWeaponPickup> baseWeaponActor;

	UPROPERTY(BlueprintReadOnly, Category = Gameplay)
	AWeaponPickup* equippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	USoundBase* SB_Pickup;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	USoundBase* SB_Drop;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float FullHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float PreviousHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	UCurveFloat* HealthCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	USoundBase* SB_DamageTaken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool DamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float FullShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float ShieldPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float PreviousShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	UCurveFloat *ShieldCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	USoundBase* SB_ShieldUp;

	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	USoundBase* SB_ShieldDown;

	UPROPERTY(EditAnywhere, Category = "Shield")
	FTimeline MyTimeline;

	UPROPERTY(EditAnywhere, Category = "Shield")
	FTimerHandle MemberTimerHandle;

	UPROPERTY(EditAnywhere, Category = "GameMode")
	FTimerHandle TimerHandle_HandleRespawn;

	float CurveFloatValue;
	float TimelineValue;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth();

	UFUNCTION(BlueprintPure, Category = "Health")
	FText GetHealthIntText();

	UFUNCTION(BlueprintPure, Category = "Shield")
	float GetShield();

	UFUNCTION(BlueprintPure, Category = "Shield")
	FText GetShieldIntText();

	UFUNCTION()
		void SetHealthValue();

	UFUNCTION()
		void SetHealthState();

	UFUNCTION()
		void SetHealthChange(float HealthValue);

	UFUNCTION()
		void SetShieldValue();

	UFUNCTION()
		void SetShieldState();

	UFUNCTION()
		void SetShieldChange(float ShieldValue);

	UFUNCTION()
		void UpdateShield(float ShieldValue);

	UFUNCTION(BlueprintPure, Category = "Health")
		bool DamageEffectAnim();

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
		void UpdateHealth(float HealthValue);

	/** get aim offsets */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
		FRotator GetAimOffsets() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
		bool GetIsShotgun();

	UFUNCTION(BlueprintPure, Category = "Weapon")
		float GetWeaponBullet();

	UFUNCTION(BlueprintPure, Category = "Weapon")
		float GetGrenadeAmount();

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Fires a projectile. */
	void OnFire();

	/** Fires a projectile. */
	void StartFire();

	void StopFire();

	UFUNCTION(BlueprintCallable)
	void ResetFire();

	void ResetAction();

	void Respawn();

	/** Fires a projectile. */
	void FireShot();

	void ProduceBullet(float offsetX, float offsetY, float offsetZ);

	//// Change between projectile and linetracing mode
	//void ChangeFireMode();

	FTimerHandle TimerHandle_HandleRefire;
	FTimerHandle TimerHandle_HandleRefireClick;
	FTimerHandle TimerHandle_HandleDeactivateThirdPerson;

	UFUNCTION(BlueprintPure)
	bool GetIsDead();

	UFUNCTION(BlueprintCallable)
	void InvertControls();

	UFUNCTION(BlueprintCallable)
	void SetSensitivity(float sensitivity);

	UFUNCTION(BlueprintCallable)
	void SetAimAssist(bool aimAssist);

	UFUNCTION(BlueprintPure)
	bool GetIsShielded();

	UFUNCTION(BlueprintPure)
	float GetTurnValue();

	UFUNCTION(BlueprintPure)
	float GetTurnAtValue();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerCrouched();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerUncrouched();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnNiagara(FVector EndLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void DamageWall(AActor* actor, FVector hit);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayHitMarkAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeCrosshair();

	UFUNCTION(BlueprintImplementableEvent)
	void DropWeapon();

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION()
	void TimelineProgressShield(float Value);

	/** Handles shielding */
	void ShieldStart();
	void ActivateThirdPerson();
	void ShieldEnd();

	void DeactivateThirdPerson();

	void ActivateFreeLookCamera();
	void DeactivateFreeLookCamera();

protected:
	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	class UAnimMontage* GrenadeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	class UAnimMontage* GrenadeMontage3P;

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	TSubclassOf<class AGrenade> GrenadeClass;

	UPROPERTY(BlueprintReadWrite, Category = "Grenade")
	class AGrenade* Grenade;

	void ThrowGrenade();

	UFUNCTION(BlueprintCallable, Category = "Grenade")
	void OnGrenadeReleased();

	UPROPERTY(EditDefaultsOnly, Category = "Bash")
	class UAnimMontage* BashMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Bash")
	class UAnimMontage* BashMontage3P;

	UPROPERTY(EditDefaultsOnly, Category = "Bash")
	class UBoxComponent* BashDamageBox;

	UPROPERTY(EditDefaultsOnly, Category = "Bash")
	USoundBase* SB_Bash;

	void Bash();

	UFUNCTION(BlueprintCallable, Category = "Bash")
	void OnBashDamage();

	UPROPERTY(EditDefaultsOnly, Category = "GroundSlam")
	TSubclassOf<class AGroundSlam> GroundSlamClass;

	UPROPERTY(BlueprintReadWrite, Category = "GroundSlam")
	class AGroundSlam* GroundSlam;

	UPROPERTY(EditDefaultsOnly, Category = "GroundSlam")
	class UAnimMontage* GroundSlamStartMontage;

	UPROPERTY(EditDefaultsOnly, Category = "GroundSlam")
	USoundBase* SB_GroundSlamCharge;

	UFUNCTION(BlueprintCallable, Category = "GroundSlam")
	void OnGroundSlamOver();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void OnDyingOver();

	UPROPERTY(EditAnywhere, Category = Gameplay)
	UCurveFloat* ViewTransitionCurve;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, Category = "CustomFunctions")
		void ChangeProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int ammoType;
	//NORMAL = 0, GRENADE = 1, SNIPER = 2, STICKY = 3, WIND = 4

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/** Handles crouching */
	void CrouchStart();
	void CrouchEnd();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	void Turn(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void JumpStart();
	void JumpEnd();

	void GroundSlamStart();
	void GroundSlamEnd();

	void ClientCrouch(bool Value);

	//// End of APawn interface

	void AimAssist(float DeltaTime);

private:
	bool isThirdPerson;
	bool canFire;
	bool canThrowGrenade;
	bool canBash;
	bool canGroundSlam;
	bool canShield;
	bool isDead; 
	bool isSlaming;
	bool isShielded;
	bool isThrowingGrenade;
	bool isBashing;
	float turnValue;
	float turnAtValue;
	float Sensitivity;
	bool isAimAssistActivated = true;
	AKotgPlayerController* PlayerControllerRef;
	UKotgInstance* myInstance;
	USkeletalMeshComponent* Mesh3P;
	FTransform FPCameraTransform;
	FTransform TPCameraTransform;
	FTimeline ViewTransitionTimeline;
	FTimeline ShieldTimeline;
	UAudioComponent* GroundSlamAudioComponent;

	//Weapon parameters and actions
	bool fromInteract;
	bool baseGun;
	USkeletalMeshComponent* baseGunMesh;
	int bullets;
	float bulletSpread;
	float gunDamage;
	USoundBase* gunSound;
	float headshotDamage;
	float minimumRecoil;
	float maximumRecoil;
	float TimeBetweenShots;
	float WeaponRange;
	bool isLineTracing;
	bool isPistol;
	bool isShotgun;
	int gunType;
	int grenadeAmount;
	//PISTOL = 0, RIFLE = 1, ROCKET = 2

public:
	int GetNbBullets();
	void Interact();
	void WeaponToss();

	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (AllowPrivateAccess = true))
	USphereComponent* interactRange;
};
