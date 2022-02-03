// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_HealthIsLow.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

UBTService_HealthIsLow::UBTService_HealthIsLow() {
	BlackboardKey.SelectedKeyName = "HealthIsLow";
}
void UBTService_HealthIsLow::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	if (AIChar->GetHealth() <= 0.5f) {
		AICon->BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, true);
	}
}
