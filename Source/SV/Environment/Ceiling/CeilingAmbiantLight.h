// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "CeilingAmbiantLight.generated.h"

class USpotLightComponent;

UCLASS()
class SV_API ACeilingAmbiantLight : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	ACeilingAmbiantLight(const FObjectInitializer& ObjectInitializer);

protected:


	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent* RootSceneComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USpotLightComponent* BackLightComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USpotLightComponent* MainLightComponent;

	FLinearColor ChooseColour();

};
