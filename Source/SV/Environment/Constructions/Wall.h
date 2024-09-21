// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "Wall.generated.h"

class UStaticMeshComponent;
class UEnvironmentDetailsComponent;
/**
 * 
 */
UCLASS()
class SV_API AWall : public AEnvironmentActor
{
	GENERATED_BODY()
	
public:

	AWall(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* WallMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UEnvironmentDetailsComponent* DetailsComponent;

};
