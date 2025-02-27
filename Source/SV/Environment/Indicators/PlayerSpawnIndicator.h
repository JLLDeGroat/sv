// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseIndicatorActor.h"
#include "PlayerSpawnIndicator.generated.h"

class UIndicatorActivatorComponent;
class UIndicatorLinkComponent;

UCLASS()
class SV_API APlayerSpawnIndicator : public ABaseIndicatorActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerSpawnIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
