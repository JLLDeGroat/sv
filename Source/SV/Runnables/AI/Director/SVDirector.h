// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SVDirector.generated.h"

class UBaseDirectorGameType;
/**
 *
 */
UCLASS()
class SV_API USVDirector : public UObject
{
    GENERATED_BODY()

public:
    USVDirector(const FObjectInitializer &ObjectInitializer);

    void ActivateSvDirector();

protected:

private:
};
