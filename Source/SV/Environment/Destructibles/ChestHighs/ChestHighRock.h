// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BaseChestHigh.h"
#include "ChestHighRock.generated.h"

class UBoxComponent;
class USphereComponent;
class UPhysicsField;
class UGeometryCollectionComponent;
class UBoxFalloff;
class UFieldSystemComponent;
class UDestructibleDetails;
class UDestructibleHitComponent;
class USkippableComponent;

UCLASS()
class SV_API AChestHighRock : public ABaseChestHigh
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AChestHighRock(const FObjectInitializer &ObjectInitializer);
    virtual FString GetRandomCollection() override;

protected:
    virtual void OnConstruction(const FTransform &Transform) override;
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    USkippableComponent *Skippable;

private:
};
