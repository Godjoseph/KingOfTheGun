// Copyright Epic Games, Inc. All Rights Reserved.

#include "KingOfTheGunGameMode.h"
#include "KingOfTheGunHUD.h"
#include "Kismet/GameplayStatics.h"
#include "KingOfTheGunCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "KotgPlayerController.h"
#include "TimerManager.h"
#include <random>
#include <AIController.h>

typedef TArray<APlayerController> DataType;

AKingOfTheGunGameMode::AKingOfTheGunGameMode()
	: Super()
{
	//active tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//change le playercontroller par défault
	PlayerControllerClass = AKotgPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	//trouver le ai controller
	static ConstructorHelpers::FClassFinder<AAIController> MyAIController(TEXT("/Game/FirstPersonCPP/Blueprints/AI/Bot_AIController_BP"));
	DefaultAIControllerClass = MyAIController.Class;

	// use our custom HUD class
	HUDClass = AKingOfTheGunHUD::StaticClass();

	mesMaps = {"ForestMap"};
}

void AKingOfTheGunGameMode::BeginPlay()
{
	Super::BeginPlay();

	myInstance = Cast<UKotgInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	SetCurrentState(EGamePlayState::EPlaying);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), mesSpawns);
	MyCharacter = Cast<AKingOfTheGunCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	mesJoueurs.Add(myInstance->GetFirstLocalPlayerController()->GetCharacter());

	playerNumber = myInstance->numberOfPlayers;
	botNumber = myInstance->numberOfBots;
	roundNumber = myInstance->numberOfRounds;
	//playerNumber = 3;
	playerStartIndex = 0;
	roundIsOver = true;
	
	RemoveLocalSplitscreenPlayersFromGame(myInstance);

	SetupAllPlayers();
	roundIsOver = false;

	SetPlayerSettings();
}

void AKingOfTheGunGameMode::SetPlayerSettings()
{
	if (myInstance)
	{
		for (int i = 0; i < mesJoueurs.Num(); i++)
		{
			Cast<AKingOfTheGunCharacter>(mesJoueurs[i])->SetSensitivity(myInstance->sensitivities[i]);
			Cast<AKingOfTheGunCharacter>(mesJoueurs[i])->SetAimAssist(myInstance->aimAssistActive[i]);
			Cast<AKingOfTheGunCharacter>(mesJoueurs[i])->nbLives = myInstance->getNumberOfLives();
			if (myInstance->inverted[i])
			{
				Cast<AKingOfTheGunCharacter>(mesJoueurs[i])->InvertControls();
			}
		}
	}
}

void AKingOfTheGunGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!roundIsOver) {
		if (mesJoueurs.Num() <= 0) {
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKingOfTheGunCharacter::StaticClass(), mesJoueurs);
		}
		int nbrAlive = 0;
		int nbrAlivePlayer = 0;
		int joueurGagnant = 0;
		int joueurGagnantVie = 0;
		int nbrPlayer = myInstance->getNumberOfPlayer();
		for (int i = 0; i < mesJoueurs.Num(); i++) {
			AKingOfTheGunCharacter* Character = Cast<AKingOfTheGunCharacter>(mesJoueurs[i]);
			if (/*!(FMath::IsNearlyZero(Character->GetHealth(), 0.001f))*/Character->nbLives > 0) {
				nbrAlive++;
				if (i < nbrPlayer) { nbrAlivePlayer++; }

				//calcul un score pour trouver le meilleur bot s'il reste aucun joueur
				int joueurVie = Character->Health + Character->nbLives * 200;
				if (joueurVie > joueurGagnantVie) {
					joueurGagnant = i;
					joueurGagnantVie = joueurVie;
				}

			}
			if ((FMath::IsNearlyZero(Character->Health, 0.001f)) && Character->nbLives > 0 && !Character->isRespawning) {
				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(this, FName("RespawnDeadPlayer"), Character);
				Character->isRespawning = true;
				GetWorld()->GetTimerManager().SetTimer(Character->TimerHandle_HandleRespawn, TimerDel, 4.f, false);
			}
		}
		if ((nbrAlive <= 1 && (mesJoueurs.Num() > 1 || UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::Y))) || nbrAlivePlayer < 1) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("END")));
			myInstance->score[joueurGagnant] += 1;
			myInstance->setWinningPlayer(joueurGagnant);
			if (myInstance->score[joueurGagnant] >= roundNumber) {
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Victoire du joueur %lld!"), joueurGagnant+1));
				Cast<AKotgPlayerController>(myInstance->GetFirstLocalPlayerController())->ShowGameOverMenu();
			}
			else {
				Cast<AKotgPlayerController>(myInstance->GetFirstLocalPlayerController())->ShowRoundEndMenu();
			}
			roundIsOver = true;
		}

	}
}



AActor* AKingOfTheGunGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{

	if (mesSpawns.Num() <= 0) {
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), mesSpawns);
		// on melange les spawns pour avoir des spawns aleatoires
		if(mesSpawns.Num()>1){ 
			int32 LastIndex = mesSpawns.Num() - 1;
			std::random_device rd;
			std::default_random_engine e1(rd());
			std::uniform_int_distribution<int> uniform_dist(0, LastIndex);
			int32 NumberOfSwap = FMath::Max(30, LastIndex); //on fait au moins 30 swaps
			for (int32 i = 0; i <= NumberOfSwap; i++) {
				mesSpawns.Swap(uniform_dist(e1), uniform_dist(e1));
			}
		}
	}

	if (mesSpawns.Num() > 0) {
		return mesSpawns[playerStartIndex % mesSpawns.Num()];
	}
	else {
		return Super::FindPlayerStart_Implementation(Player, IncomingName);
	}
}
/*
AActor* AKingOfTheGunGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("choose"));
	return mesSpawns[0];
}*/

EGamePlayState AKingOfTheGunGameMode::GetCurrentState()
{
	return CurrentState;
}

void AKingOfTheGunGameMode::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void AKingOfTheGunGameMode::RemoveLocalSplitscreenPlayersFromGame(UGameInstance* CurrentGameInstance)
{
	const int32 MaxSplitScreenPlayers = 4;
	ULocalPlayer* PlayersToRemove[MaxSplitScreenPlayers];
	int32 RemIndex = 0;

	for (FConstPlayerControllerIterator Iterator = GEngine->GameViewport->GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		// Remove only local split screen players
		APlayerController* Controller = Cast<APlayerController>(*Iterator);
		if (Controller && Controller->IsLocalController() && !Controller->IsPrimaryPlayer())
		{
			ULocalPlayer* ExPlayer = Cast<ULocalPlayer>(Controller->Player);
			if (ExPlayer)
			{
				PlayersToRemove[RemIndex++] = ExPlayer;
				Controller->PawnPendingDestroy(Controller->GetPawn());
			}
		}
	}

	for (int32 i = 0; i < RemIndex; ++i)
	{
		//GEngine->GameViewport->RemovePlayer(PlayersToRemove[i]); // GameViewport::RemovePlayer was Deprecated in 4.4
		CurrentGameInstance->RemoveLocalPlayer(PlayersToRemove[i]);
	}
}

void AKingOfTheGunGameMode::LoadRandomMap()
{
	int arrLength = mesMaps.Num();
	std::random_device rd;
	std::default_random_engine e1(rd());
	std::uniform_int_distribution<int> uniform_dist(0, arrLength-1);
	int i = uniform_dist(e1);
	RemoveLocalSplitscreenPlayersFromGame(myInstance);
	UGameplayStatics::OpenLevel((UObject*)myInstance, mesMaps[i]);
}

void AKingOfTheGunGameMode::HandleNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
		case EGamePlayState::EPlaying:
			break;
		case EGamePlayState::EGameOver:
		{
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		}
			break;
		case EGamePlayState::EUnknown:
			break;
		default:
			break;
	}
}

void AKingOfTheGunGameMode::SetupAllPlayers()
{
	class UWorld* const world = GetWorld();

	if (world != nullptr)
	{
		for (int i = 1; i < playerNumber + botNumber; i++) 
		{
			playerStartIndex = i;
			AActor* spawn = mesSpawns[playerStartIndex % mesSpawns.Num()];
			if (i < playerNumber) 
			{
				class APlayerController* NewPlayerController = Cast<APlayerController>(UGameplayStatics::CreatePlayer(world, playerStartIndex));
				Cast<AKotgPlayerController>(NewPlayerController)->SetPlayerIndex(playerStartIndex);
				mesJoueurs.Add(NewPlayerController->GetCharacter());
				Cast<AKotgPlayerController>(NewPlayerController)->SetPlayerIndex(playerStartIndex);
			}
			else 
			{
				APawn* bot = this->GetWorld()->SpawnActor<APawn>(DefaultPawnClass, spawn->GetTargetLocation(), spawn->GetActorRotation());
				AController* NewController = GetWorld()->SpawnActor<AController>(DefaultAIControllerClass, spawn->GetTargetLocation(), spawn->GetActorRotation());
				NewController->Possess(bot);
				mesJoueurs.Add(bot);
			}
		}
	}
}

void AKingOfTheGunGameMode::RespawnDeadPlayer(AKingOfTheGunCharacter* Player)
{
	GetWorldTimerManager().ClearTimer(Player->TimerHandle_HandleRespawn);
	AKotgPlayerController* PlayerControllerRef = Cast<AKotgPlayerController>(Player->GetController());

	std::random_device rd;
	std::default_random_engine e1(rd());

	if (mesSpawns.Num() <= 0) {
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), mesSpawns);
		// on melange les spawns pour avoir des spawns aleatoires
		if (mesSpawns.Num() > 1) {
			int32 LastIndex = mesSpawns.Num() - 1;

			std::uniform_int_distribution<int> uniform_dist(0, LastIndex);
			int32 NumberOfSwap = FMath::Max(30, LastIndex); //on fait au moins 30 swaps
			for (int32 i = 0; i <= NumberOfSwap; i++) {
				mesSpawns.Swap(uniform_dist(e1), uniform_dist(e1));
			}
		}
	}
	AActor* spawn;
	std::uniform_int_distribution<int> uniform_dist(0, mesSpawns.Num() - 1);

	spawn = mesSpawns[uniform_dist(e1)];

	Player->SetActorLocation(spawn->GetActorLocation());
	Player->SetActorRotation(spawn->GetActorRotation());
	Player->Respawn();
}
