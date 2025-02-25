// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BaseChestHigh.h"
#include "ChestHighFence.generated.h"

class UVaultableComponent;

UCLASS()
class SV_API AChestHighFence : public ABaseChestHigh
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AChestHighFence(const FObjectInitializer &ObjectInitializer);

    virtual FString GetRandomCollection() override;

protected:
    virtual void OnConstruction(const FTransform &Transform) override;
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UVaultableComponent *Vaultable;

private:
};
