// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RemoveThreat.h"
#include "Bot_AIController.h"

void UBTService_RemoveThreat::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());

	if (AICon->threat > 0) {
		AICon->threat--;
	}
}
