// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseDD.generated.h"

/**
 *
 */
UCLASS()
class SV_API UBaseDD : public UObject
{
    GENERATED_BODY()

public:
    UBaseDD(const FObjectInitializer &ObjectInitializer) : UObject(ObjectInitializer) {}
};
