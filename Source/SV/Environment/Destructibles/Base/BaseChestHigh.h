// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../EnvironmentActor.h"
#include "BaseChestHigh.generated.h"

class UBoxComponent;
class USphereComponent;
class UPhysicsField;
class UGeometryCollectionComponent;
class UBoxFalloff;
class UFieldSystemComponent;
class UDestructibleDetails;
class UDestructibleHitComponent;
class USkippableComponent;
class UCullingField;
class UUniformInteger;

UCLASS()
class SV_API ABaseChestHigh : public AEnvironmentActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABaseChestHigh(const FObjectInitializer &ObjectInitializer);

    virtual FString GetRandomCollection() { return "INVALID"; };
    void ReEnableForceOnHit();

protected:
    virtual void OnConstruction(const FTransform &Transform) override;
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UGeometryCollectionComponent *DestructibleMesh;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UBoxFalloff *StaticBoxFallOff;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UBoxComponent *DisableBoxComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UFieldSystemComponent *KillFieldSystem;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UDestructibleDetails *DestructibleDetails;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UDestructibleHitComponent *DestructibleHitComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Health;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bDisableAreaAtStart;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UCullingField *CullingField;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UUniformInteger *UniformInt;
};
