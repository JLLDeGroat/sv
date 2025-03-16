// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PostMovementComponent.generated.h"

UINTERFACE(BlueprintType)
class SV_API UPostMovementComponent : public UInterface
{
    GENERATED_BODY()
};

class SV_API IPostMovementComponent
{

    GENERATED_BODY()

public:
    virtual void Setup(AActor *movedActor) = 0;
    virtual void BeginComponent() = 0;
    virtual void Finish() = 0;
};
