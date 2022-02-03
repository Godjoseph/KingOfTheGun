// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AFire::AFire()
{
    MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("My box Component"));
    MyBoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
    RootComponent = MyBoxComponent;

    Fire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("My Fire"));
    Fire->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Fire->SetupAttachment(RootComponent);

    MyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFire::OnOverlapBegin);
    MyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFire::OnOverlapEnd);

    bCanApplyDamage = true;
    fireDamage = 1.0f;
}

void AFire::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
    {
        bCanApplyDamage = true;
        MyCharacters.Add(Cast<AActor>(OtherActor));
        MyHit = SweepResult;
        GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AFire::ApplyFireDamage, 1.0f, true, 0.0f);
    }
}

void AFire::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bCanApplyDamage = true;
    MyCharacters.Remove(Cast<AActor>(OtherActor));
    GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AFire::ApplyFireDamage()
{
    if (bCanApplyDamage)
    {
        for (int i = 0; i < MyCharacters.Num(); i++)
        {
            UGameplayStatics::ApplyPointDamage(MyCharacters[i], fireDamage, GetActorLocation(), MyHit, nullptr, this, FireDamageType);
        }
    }
}
