// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseIndicatorActor.h"
#include "HealthKitUseIndicator.generated.h"

class UIndicatorClickComponent;
class UIndicatorLinkComponent;
/**
 *
 */
UCLASS()
class SV_API AHealthKitUseIndicator : public ABaseIndicatorActor
{
	GENERATED_BODY()

public:

	AHealthKitUseIndicator();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UIndicatorClickComponent* ClickComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UIndicatorLinkComponent* LinkComponent;
};
