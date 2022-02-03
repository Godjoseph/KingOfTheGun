// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());
	
	if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - AICon->timeLastShot >= 1) {
		UCharacterMovementComponent* charMovement = AIChar->GetCharacterMovement();
		charMovement->StopActiveMovement();

		AICon->timeLastShot = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		AIChar->StartFire();

		FTimerHandle UnusedHandle;
		AIChar->GetWorldTimerManager().SetTimer(UnusedHandle, AIChar, &AKingOfTheGunCharacter::StopFire, 1, false);
		
	}

	return EBTNodeResult::Succeeded;
}