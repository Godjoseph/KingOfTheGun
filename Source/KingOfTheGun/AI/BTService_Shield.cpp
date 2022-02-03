// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Shield.h"
#include "Bot_AIController.h"

UBTService_Shield::UBTService_Shield() {
	BlackboardKey.SelectedKeyName = "useShield";
}
void UBTService_Shield::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());

	if (AICon->threat >= 10) {
		AICon->BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, true);
	}
	else {
		AICon->BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, false);
	}
}
