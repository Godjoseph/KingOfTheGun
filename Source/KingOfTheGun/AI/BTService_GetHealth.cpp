// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetHealth.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

UBTService_GetHealth::UBTService_GetHealth() {
	BlackboardKey.SelectedKeyName = "health";
}
void UBTService_GetHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	AICon->BlackboardComp->SetValueAsFloat(BlackboardKey.SelectedKeyName, AIChar->GetHealth());
}
