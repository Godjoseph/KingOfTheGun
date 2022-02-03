// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EquipWeapon.h"

EBTNodeResult::Type UBTTask_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());

	AICon->EquipWeapon();

	return EBTNodeResult::Succeeded;
}