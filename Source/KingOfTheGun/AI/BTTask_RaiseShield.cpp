// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RaiseShield.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

EBTNodeResult::Type UBTTask_RaiseShield::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	if (AIChar->GetIsShielded() == false) {
		AIChar->ShieldStart();

		FTimerHandle UnusedHandle;
		AIChar->GetWorldTimerManager().SetTimer(UnusedHandle, AIChar, &AKingOfTheGunCharacter::ShieldEnd, FMath::RandRange(1, 5), false);

		AICon->BlackboardComp->SetValueAsBool("usingShield", true);
	}
	
	return EBTNodeResult::Succeeded;
}
