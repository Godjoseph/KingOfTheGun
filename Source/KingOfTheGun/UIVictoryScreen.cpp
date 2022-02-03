// Fill out your copyright notice in the Description page of Project Settings.


#include "UIVictoryScreen.h"
#include "Engine.h"
//#include "KingOfTheGunGameMode.h"
#include "KotgPlayerController.h"
#include "KotgInstance.h"
#include "Components/Widget.h"


void UUIVictoryScreen::NativeConstruct() {
	Super::NativeConstruct();

	if (!NextButton->OnClicked.IsBound()) NextButton->OnClicked.AddDynamic(this, &UUIVictoryScreen::OnClick);

	UKotgInstance* myInstance = Cast<UKotgInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//ScoreP1->SetText(FText::FromString(FString::Printf(TEXT("%lld"), myInstance->score[0])));
	//ScoreP2->SetText(FText::FromString(FString::Printf(TEXT("%lld"), myInstance->score[1])));
	//ScoreP3->SetText(FText::FromString(FString::Printf(TEXT("%lld"), myInstance->score[2])));
	//ScoreP4->SetText(FText::FromString(FString::Printf(TEXT("%lld"), myInstance->score[3])));
	//SetVisibility(ESlateVisibility::Visible);
	//NextButton->SetVisibility(ESlateVisibility::Visible);
	//NextButton->SetKeyboardFocus();
}


void UUIVictoryScreen::OnClick() {
	UKotgInstance* myInstance = Cast<UKotgInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Cast<AKotgPlayerController>(myInstance->GetFirstLocalPlayerController())->RemoveRoundEndMenu();

	//AKingOfTheGunGameMode* GameMode = Cast<AKingOfTheGunGameMode>(GetWorld()->GetAuthGameMode());
	//GameMode->LoadRandomMap();
}