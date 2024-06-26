// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "NaturalRoad.generated.h"

/**
 * 
 */
UCLASS()
class SV_API ANaturalRoad : public AEnvironmentActor
{
	GENERATED_BODY()
	
public:

	ANaturalRoad(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDecalComponent* RoadDecalComponent;

};
