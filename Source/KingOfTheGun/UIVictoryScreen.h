// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Button.h"
#include "Components/TextBlock.h"
#include "UIVictoryScreen.generated.h"

/**
 * 
 */
UCLASS()
class KINGOFTHEGUN_API UUIVictoryScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget)) UButton* NextButton;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ScoreP1;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ScoreP2;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ScoreP3;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ScoreP4;
	UFUNCTION() void OnClick();

};
