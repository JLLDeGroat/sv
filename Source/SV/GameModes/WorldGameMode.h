// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WorldGameMode.generated.h"

class UWorldMapWallManager;
class UWorldDirectionManager;
/**
 * 
 */
UCLASS()
class SV_API AWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AWorldGameMode();
	
	virtual void BeginPlay() override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UWorldMapWallManager* RouteWallManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UWorldDirectionManager* DirectionManager;

private:


};
