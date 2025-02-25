// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DestructibleHitComponent.generated.h"

class UFieldSystemComponent;
class UGeometryCollectionComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UDestructibleHitComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDestructibleHitComponent(const FObjectInitializer &ObjectInitializer);

    void TestMethod(FVector loc, float f1, float f2, int32 i1);

    void ComponentWasHit(float damage, FVector forwardVector, FVector hitLoc, float magnitude, float radius, int32 iterations = 25);

    void AddComponentToDisableOnDeath(UGeometryCollectionComponent *component);

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UFieldSystemComponent *KillFieldSystem;

private:
    FTimerHandle DestroyedTimer;
};
