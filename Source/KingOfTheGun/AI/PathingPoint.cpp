// Fill out your copyright notice in the Description page of Project Settings.


#include "PathingPoint.h"

// Sets default values
APathingPoint::APathingPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathingPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathingPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int APathingPoint::GetOrder() {
	if (this != nullptr) {
		return this->order;
	}
	else {
		return 0;
	}
}

