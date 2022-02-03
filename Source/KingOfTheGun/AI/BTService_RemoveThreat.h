// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_RemoveThreat.generated.h"

/**
 * 
 */
UCLASS()
class KINGOFTHEGUN_API UBTService_RemoveThreat : public UBTService_BlackboardBase
{
	GENERATED_BODY()
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
