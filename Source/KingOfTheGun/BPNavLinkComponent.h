// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavLinkComponent.h"
#include "BPNavLinkComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KINGOFTHEGUN_API UBPNavLinkComponent : public UNavLinkComponent
{
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable)
		void SetupLink(FVector Left, FVector Right, ENavLinkDirection::Type Direction);

	UFUNCTION(BlueprintCallable)
		void SetNavLinkEnabled(bool enabled);
};
