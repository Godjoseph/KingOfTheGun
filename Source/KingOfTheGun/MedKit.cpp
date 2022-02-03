// Fill out your copyright notice in the Description page of Project Settings.


#include "MedKit.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMedKit::AMedKit()
{
    OnActorBeginOverlap.AddDynamic(this, &AMedKit::OnOverlap);
}

void AMedKit::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		MyCharacter = Cast<AKingOfTheGunCharacter>(OtherActor);

		if (MyCharacter && MyCharacter->GetHealth() < 1.0f)
		{
			if (SB_Medkit)
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SB_Medkit, GetActorLocation());
			}
			MyCharacter->UpdateHealth(healthHealing);
			Destroy();
		}
	}
}
