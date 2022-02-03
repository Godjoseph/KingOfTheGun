// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Jump.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

EBTNodeResult::Type UBTTask_Jump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	AIChar->Jump();

	FTimerHandle UnusedHandle;
	AIChar->GetWorldTimerManager().SetTimer(UnusedHandle, AIChar, &AKingOfTheGunCharacter::StopJumping, 0.5f, false);

	return EBTNodeResult::Succeeded;
}
