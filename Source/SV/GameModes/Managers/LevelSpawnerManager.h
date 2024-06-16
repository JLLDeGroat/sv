// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelSpawnerManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API ULevelSpawnerManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevelSpawnerManager();

	void SpawnMajorWallAt(FVector loc, FRotator rot = FRotator(0));

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
