// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BasePostMovementComponent.h"
#include "Interfaces/PostMovementComponent.h"
#include "PostMovementFogHandler.generated.h"

/**
 *
 */
UCLASS()
class SV_API UPostMovementFogHandler : public UBasePostMovementComponent, public IPostMovementComponent
{
    GENERATED_BODY()

public:
    UPostMovementFogHandler();

    virtual void Setup(AActor *movedActor) override;
    virtual void BeginComponent() override;
    virtual void Finish() override;

protected:
    void RemoveFogAroundCharacter();
};
