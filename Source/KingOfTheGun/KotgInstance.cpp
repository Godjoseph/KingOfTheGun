// Fill out your copyright notice in the Description page of Project Settings.


#include "KotgInstance.h"
#include "EngineSettings/Classes/GameMapsSettings.h"

void UKotgInstance::Init() {
	score.Init(0, 4);
	sensitivities.Init(1.0, 4);
	inverted.Init(false, 4);
	aimAssistActive.Init(true, 4);

	numberOfPlayers = 1;
	numberOfBots = 3;
	numberOfRounds = 4;
	numberOfLives = 2;
}

void UKotgInstance::resetAllScores()
{
	score[0] = 0;
	score[1] = 0;
	score[2] = 0;
	score[3] = 0;
}

void UKotgInstance::setNumberOfPlayer(int nbr)
{
	numberOfPlayers = nbr;
}

int UKotgInstance::getNumberOfPlayer()
{
	return numberOfPlayers;
}

void UKotgInstance::OffsetGamepadID(bool Offset)
{
	UGameMapsSettings* Settings = const_cast<UGameMapsSettings*>(GetDefault<UGameMapsSettings>());

	if (Settings == nullptr) { return; }

	Settings->bOffsetPlayerGamepadIds = Offset;
}

bool UKotgInstance::GetOffset()
{
	UGameMapsSettings* Settings = const_cast<UGameMapsSettings*>(GetDefault<UGameMapsSettings>());

	if (Settings == nullptr) { return false; }

	return Settings->bOffsetPlayerGamepadIds;
}
