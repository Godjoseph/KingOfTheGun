// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "KingOfTheGunCharacter.h"
#include "KotgInstance.h"
#include "AIController.h"
#include "KingOfTheGunGameMode.generated.h"

UENUM()
enum class EGamePlayState
{
	EPlaying,
	EGameOver,
	EUnknown
};

UCLASS(minimalapi)
class AKingOfTheGunGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKingOfTheGunGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetPlayerSettings();

	virtual void Tick(float DeltaTime) override;

	//AActor* FindPlayerStart(AController* Player, const FString& IncomingName = TEXT(""));
	//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName = TEXT("")) override;

	AKingOfTheGunCharacter* MyCharacter;
	AKingOfTheGunCharacter* MyCharacter_P2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
		TSubclassOf<AAIController> DefaultAIControllerClass;


	UFUNCTION(BlueprintPure, Category = "Health")
	EGamePlayState GetCurrentState();

	void SetCurrentState(EGamePlayState NewState);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Play Mode")
	bool isTwoPlayerMode;

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	void RemoveLocalSplitscreenPlayersFromGame(UGameInstance* CurrentGameInstance);

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		void LoadRandomMap();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Play Mode")
		TArray<FName> mesMaps;

private:
	EGamePlayState CurrentState;

	void HandleNewState(EGamePlayState NewState);

	void SetupAllPlayers();

	UFUNCTION()
	void RespawnDeadPlayer(AKingOfTheGunCharacter* Player);

	UPROPERTY()
	TArray<AActor*> mesSpawns;
	UPROPERTY()
	TArray<AActor*> mesJoueurs;

	int playerNumber;
	int botNumber;
	int roundNumber;
	int playerStartIndex;
	bool roundIsOver;

	UKotgInstance* myInstance;

	//TArray<FTimerHandle> TimerHandle_HandleRespawn;
};



