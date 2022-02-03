// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ReceivedDamage.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

UBTService_ReceivedDamage::UBTService_ReceivedDamage() {
	BlackboardKey.SelectedKeyName = "takingDamage";
}
void UBTService_ReceivedDamage::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());


	if (AICon->timeSinceLastHit != 0 && (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - AICon->timeSinceLastHit <= 3)) {
		AICon->BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, true);
	}
	else {
		AICon->BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, false);
	}
}
