// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "MudFloor.generated.h"


class UEnvironmentDetailsComponent;

UCLASS()
class SV_API AMudFloor : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	AMudFloor(const FObjectInitializer& ObjectInitializer);


protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* FloorMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UEnvironmentDetailsComponent* EnvironmentDetails;
};
