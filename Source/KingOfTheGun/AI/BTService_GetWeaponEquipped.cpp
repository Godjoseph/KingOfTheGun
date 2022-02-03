// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetWeaponEquipped.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

UBTService_GetWeaponEquipped::UBTService_GetWeaponEquipped() {
	BlackboardKey.SelectedKeyName = "weaponEquipped";
}
void UBTService_GetWeaponEquipped::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	FString weaponName = AIChar->equippedWeaponActor->GetName();

	AICon->BlackboardComp->SetValueAsString(BlackboardKey.SelectedKeyName, weaponName);
}

