// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Interfaces/ClothingItem.h"
#include "Clothing.generated.h"

class USceneComponent;
class UClothingStatsComponent;
class UClothingAttachComponent;
class UStaticMeshComponent;
class UClothingDamageReceiveComponent;

UCLASS()
class SV_API AClothing : public AActor, public IClothingItem
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AClothing(const FObjectInitializer &ObjectInitializer);

    virtual EClothingType GetClothingType() override { return ClothingType; }
    virtual EClothingGrade GetClothingGrade() override { return ClothingGrade; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void SetupClothingMeshComponent(UStaticMeshComponent *component);

    UPROPERTY()
    UClothingStatsComponent *StatsComponent;

    UPROPERTY()
    UClothingAttachComponent *AttachComponent;

    UPROPERTY()
    UClothingDamageReceiveComponent *DamageReceiveComponent;

    UPROPERTY()
    EClothingType ClothingType;

    UPROPERTY()
    EClothingGrade ClothingGrade;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
