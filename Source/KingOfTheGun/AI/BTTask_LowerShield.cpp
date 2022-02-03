// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LowerShield.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

EBTNodeResult::Type UBTTask_LowerShield::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	AIChar->ShieldEnd();
	AICon->BlackboardComp->SetValueAsBool("usingShield", false);

	return EBTNodeResult::Succeeded;
}
