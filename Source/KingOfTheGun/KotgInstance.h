// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KotgInstance.generated.h"

/**
 * 
 */
UCLASS()
class KINGOFTHEGUN_API UKotgInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UPROPERTY()
	int numberOfPlayers;
	int numberOfBots;
	int numberOfRounds;
	int winningPlayer;
	int numberOfLives;


	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		void resetAllScores();

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	void setNumberOfPlayer(int nbr);
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	int getNumberOfPlayer();

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		void setNumberOfBot(int nbr) { numberOfBots = nbr; };
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		int getNumberOfBot() { return numberOfBots; };

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		void setNumberOfRound(int nbr) { numberOfRounds = nbr; };
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		int getNumberOfRound() { return numberOfRounds; };
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		void setWinningPlayer(int nbr) { winningPlayer = nbr; };
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		int getWinningPlayer() { return winningPlayer; };
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		void setNumberOfLives(int nbr) { numberOfLives = nbr; };
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		int getNumberOfLives() { return numberOfLives; };

	// Sets "bOffsetPlayerGamepadIds" on UGameMapSettings
	UFUNCTION(BlueprintCallable, Category = "Gamepad")
		static void OffsetGamepadID(bool Offset);
	UFUNCTION(BlueprintCallable, Category = "Gamepad")
		static bool GetOffset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> sensitivities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> inverted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> aimAssistActive;
};
