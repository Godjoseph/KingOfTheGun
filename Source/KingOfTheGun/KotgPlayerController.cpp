// Fill out your copyright notice in the Description page of Project Settings.


#include "KotgPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "KingOfTheGunGameMode.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

AKotgPlayerController::AKotgPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//pause menu
	//Super::APlayerController(ObjectInitializer);
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerSelectMenu(TEXT("/Game/UI/PlayerSelectMenu"));
	WidgetPauseTemplate = PlayerSelectMenu.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> RoundEndMenu(TEXT("/Game/UI/RoundEndMenu"));
	WidgetRoundEndTemplate = RoundEndMenu.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverMenu(TEXT("/Game/UI/GameOverMenu"));
	WidgetGameOverTemplate = GameOverMenu.Class;

	myPlayerIndex = 0;
}

void AKotgPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAction("Pause", IE_Pressed, this, &AKotgPlayerController::ShowPauseMenu); //marche pas pour l'instant
}

void AKotgPlayerController::ShowPauseMenu()
{
	if (IsPrimaryPlayer()) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("PauseWithKotgController")));
		if (WidgetPauseTemplate) {
			if (!WidgetPauseInstance) {
				WidgetPauseInstance = CreateWidget(this, WidgetPauseTemplate);
			}
		}
		if (!WidgetPauseInstance->GetIsVisible())
		{
			WidgetPauseInstance->AddToViewport();
		}
		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(WidgetPauseInstance->GetCachedWidget());
		SetInputMode(Mode);
		bShowMouseCursor = true;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("EchecPause")));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%lld"), myPlayerIndex));
		AKotgPlayerController* firstPlayer = Cast<AKotgPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (firstPlayer) {
			firstPlayer->ShowPauseMenu();
		}
	}
}

void AKotgPlayerController::ShowRoundEndMenu()
{
	if (WidgetRoundEndTemplate) {
		if (!WidgetRoundEndInstance) {
			WidgetRoundEndInstance = CreateWidget(this, WidgetRoundEndTemplate);
		}
	}
	if (!WidgetRoundEndInstance->GetIsVisible())
	{
		WidgetRoundEndInstance->AddToViewport();
	}
	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(WidgetRoundEndInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}

void AKotgPlayerController::RemoveRoundEndMenu()
{
	WidgetRoundEndInstance->RemoveFromViewport();
	WidgetRoundEndInstance = nullptr;
	FInputModeGameOnly GameMode;
	SetInputMode(GameMode);
	bShowMouseCursor = false;

	AKingOfTheGunGameMode* myGameMode = Cast<AKingOfTheGunGameMode>(GetWorld()->GetAuthGameMode());
	myGameMode->LoadRandomMap();
}

void AKotgPlayerController::ShowGameOverMenu()
{
	if (WidgetGameOverTemplate) {
		if (!WidgetGameOverInstance) {
			WidgetGameOverInstance = CreateWidget(this, WidgetGameOverTemplate);
		}
	}
	if (!WidgetGameOverInstance->GetIsVisible())
	{
		WidgetGameOverInstance->AddToViewport();
	}
	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(WidgetGameOverInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}

int AKotgPlayerController::GetPlayerIndex()
{
	return myPlayerIndex;
}

void AKotgPlayerController::SetPlayerIndex(int x)
{
	myPlayerIndex = x;
}
