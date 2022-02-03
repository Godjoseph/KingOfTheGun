// Fill out your copyright notice in the Description page of Project Settings.

#include "FindEnemyQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "../KingOfTheGunCharacter.h"
#include "Bot_AIController.h"

void UFindEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ABot_AIController* AICon = Cast<ABot_AIController>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));

	if (AICon)
	{
		AKingOfTheGunCharacter* enemy = Cast<AKingOfTheGunCharacter>(AICon->GetSeeingPawn());
		if (enemy && enemy->GetIsDead() != true) {
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, AICon->GetSeeingPawn());
		}
	}
}