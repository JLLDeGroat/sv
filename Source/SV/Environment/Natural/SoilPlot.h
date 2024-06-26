// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "SoilPlot.generated.h"

/**
 * 
 */
UCLASS()
class SV_API ASoilPlot : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	ASoilPlot(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;


protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* SoilPlotMeshComponent;

};
