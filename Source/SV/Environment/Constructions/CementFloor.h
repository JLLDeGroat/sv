// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "CementFloor.generated.h"

class UStaticMeshComponent;

UCLASS()
class SV_API ACementFloor : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	ACementFloor(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* FloorMeshComponent;

};
