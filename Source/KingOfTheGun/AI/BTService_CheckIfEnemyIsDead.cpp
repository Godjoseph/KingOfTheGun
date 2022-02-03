// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckIfEnemyIsDead.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

UBTService_CheckIfEnemyIsDead::UBTService_CheckIfEnemyIsDead() {
	BlackboardKey.SelectedKeyName = "Enemy";
}
void UBTService_CheckIfEnemyIsDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	AKingOfTheGunCharacter* Enemy = Cast<AKingOfTheGunCharacter>(AICon->BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (Enemy != nullptr) {
		if (Enemy->GetIsDead() == true) {
			AICon->BlackboardComp->ClearValue(BlackboardKey.SelectedKeyName);
		}
	}
}