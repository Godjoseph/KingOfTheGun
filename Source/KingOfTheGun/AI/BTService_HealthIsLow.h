// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_HealthIsLow.generated.h"

/**
 * 
 */
UCLASS()
class KINGOFTHEGUN_API UBTService_HealthIsLow : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:
	UBTService_HealthIsLow();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
