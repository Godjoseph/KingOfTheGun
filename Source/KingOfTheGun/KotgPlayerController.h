// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KotgPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KINGOFTHEGUN_API AKotgPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	AKotgPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void SetupInputComponent() override;

	//menu pause
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> WidgetPauseTemplate;

	UPROPERTY()
		UUserWidget* WidgetPauseInstance;

	//round end menu
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> WidgetRoundEndTemplate;

	UPROPERTY()
		UUserWidget* WidgetRoundEndInstance;

	//game over menu
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> WidgetGameOverTemplate;

	UPROPERTY()
		UUserWidget* WidgetGameOverInstance;

public:
	void ShowPauseMenu();

	void ShowRoundEndMenu();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void RemoveRoundEndMenu();

	void ShowGameOverMenu();

	int myPlayerIndex;
	int GetPlayerIndex();
	void SetPlayerIndex(int x);

};
