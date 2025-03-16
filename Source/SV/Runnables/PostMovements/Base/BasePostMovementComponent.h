// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Utilities/GridUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "BasePostMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class SV_API UBasePostMovementComponent : public UObject
{
    GENERATED_BODY()

public:
    UBasePostMovementComponent();

protected:
    UPROPERTY()
    AActor *MovedActor;
};
