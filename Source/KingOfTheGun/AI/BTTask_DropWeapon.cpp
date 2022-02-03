// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DropWeapon.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

EBTNodeResult::Type UBTTask_DropWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	AIChar->WeaponToss();

	AICon->BlackboardComp->SetValueAsBool("weaponEquipped", false);

	return EBTNodeResult::Succeeded;
}
