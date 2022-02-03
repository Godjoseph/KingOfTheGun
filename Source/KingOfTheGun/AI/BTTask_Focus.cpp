// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Focus.h"
#include "Bot_AIController.h"

EBTNodeResult::Type UBTTask_Focus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());

	AActor* Enemy = Cast<AActor>(AICon->BlackboardComp->GetValueAsObject("Enemy"));

	if (Enemy != nullptr) {
		AICon->SetFocus(Enemy);
	}

	return EBTNodeResult::Succeeded;
}
