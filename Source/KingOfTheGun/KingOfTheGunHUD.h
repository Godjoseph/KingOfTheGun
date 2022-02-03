// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KingOfTheGunHUD.generated.h"

UCLASS()
class AKingOfTheGunHUD : public AHUD
{
	GENERATED_BODY()

public:
	AKingOfTheGunHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	UPROPERTY(EditAnywhere, Category = "Health")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Health")
	class UUserWidget* CurrentWidget;
};
