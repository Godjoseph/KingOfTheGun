// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_GetWeaponEquipped.generated.h"

/**
 * 
 */
UCLASS()
class KINGOFTHEGUN_API UBTService_GetWeaponEquipped : public UBTService_BlackboardBase
{
	GENERATED_BODY()
protected:
	UBTService_GetWeaponEquipped();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
