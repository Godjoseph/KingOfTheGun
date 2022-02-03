// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Jump.h"
#include "Bot_AIController.h"
#include "Math/UnrealMathUtility.h"

UBTService_Jump::UBTService_Jump() {
	BlackboardKey.SelectedKeyName = "jump";
}
void UBTService_Jump::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());

	if (FMath::RandRange(1, 10) == 1) {
		AICon->BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, true);
	}
	else {
		AICon->BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, false);
	}
}
