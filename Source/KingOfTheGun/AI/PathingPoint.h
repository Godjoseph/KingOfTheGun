// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathingPoint.generated.h"

UCLASS()
class KINGOFTHEGUN_API APathingPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathingPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetOrder();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	int order = 0;
};
