// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathingPoint.h"
#include "PathingPoint.h"
#include "Bot_AIController.h"
#include "../KingOfTheGunCharacter.h"

EBTNodeResult::Type UBTTask_FindPathingPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ABot_AIController* AICon = Cast<ABot_AIController>(OwnerComp.GetAIOwner());
	AKingOfTheGunCharacter* AIChar = Cast<AKingOfTheGunCharacter>(AICon->GetPawn());

	TArray<AActor*> allPathingPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APathingPoint::StaticClass(), allPathingPoints);

	if (allPathingPoints.Num() > 0) {
		if (AICon->currentPathingPoint->GetOrder() == 0) {//Find closest pathing point
			AActor* closestPoint = nullptr;

			for (AActor* pathingPoint : allPathingPoints) {
				if (closestPoint == nullptr) {
					closestPoint = pathingPoint;
				}
				else {
					if (AIChar->GetDistanceTo(pathingPoint) < AIChar->GetDistanceTo(closestPoint)) {
						closestPoint = pathingPoint;
					}
				}
			}
			if (closestPoint != nullptr) {
				AICon->BlackboardComp->SetValueAsVector("pathingPoint", closestPoint->GetActorLocation());
				AICon->currentPathingPoint = Cast<APathingPoint>(closestPoint);
			}
		}
		else {
			if (AIChar->GetDistanceTo(AICon->currentPathingPoint) <= 150.0f) {
				int nextOrder = AICon->currentPathingPoint->GetOrder() + 1;
				if (nextOrder > allPathingPoints.Num()) {
					nextOrder = 1;
				}

				for (AActor* pathingPoint : allPathingPoints) {
					APathingPoint* pathPoint = Cast<APathingPoint>(pathingPoint);
					if (pathPoint->GetOrder() == nextOrder) {
						AICon->BlackboardComp->SetValueAsVector("pathingPoint", pathPoint->GetActorLocation());
						AICon->currentPathingPoint = pathPoint;
					}
				}
			}
			else {
				AICon->BlackboardComp->SetValueAsVector("pathingPoint", AICon->currentPathingPoint->GetActorLocation());
			}
		}

		return EBTNodeResult::Succeeded;
	}
	else {
		return EBTNodeResult::Failed;
	}
}
