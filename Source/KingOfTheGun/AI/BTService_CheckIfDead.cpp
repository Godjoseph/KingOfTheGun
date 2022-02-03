// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckIfDead.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

UBTService_CheckIfDead::UBTService_CheckIfDead() {
	BlackboardKey.SelectedKeyName = "IsDead";
}
void UBTService_CheckIfDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());
	
	if (AIChar->GetIsDead() == true) {
		AICon->BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, true);
	}
}

