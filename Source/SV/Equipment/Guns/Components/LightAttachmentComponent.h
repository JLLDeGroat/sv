// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "LightAttachmentComponent.generated.h"

class USpotLightComponent;

/**
 * 
 */
UCLASS()
class SV_API ULightAttachmentComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	ULightAttachmentComponent();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) USpotLightComponent* DirectionalLightComponent;
};
