// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_Shield.generated.h"

/**
 * 
 */
UCLASS()
class KINGOFTHEGUN_API UBTService_Shield : public UBTService_BlackboardBase
{
	GENERATED_BODY()
protected:
	UBTService_Shield();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
