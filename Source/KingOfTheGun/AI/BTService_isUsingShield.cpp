// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_isUsingShield.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

UBTService_isUsingShield::UBTService_isUsingShield() {
	BlackboardKey.SelectedKeyName = "usingShield";
}
void UBTService_isUsingShield::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	AICon->BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, AIChar->GetIsShielded());
}
