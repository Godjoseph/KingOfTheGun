// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetAmmo.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

UBTService_GetAmmo::UBTService_GetAmmo() {
	BlackboardKey.SelectedKeyName = "ammo";
}
void UBTService_GetAmmo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	AICon->BlackboardComp->SetValueAsInt(BlackboardKey.SelectedKeyName, AIChar->GetNbBullets());
}
