// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Clothing.h"
#include "ClothingHelmet.generated.h"

class USceneComponent;

UCLASS()
class SV_API AClothingHelmet : public AClothing
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AClothingHelmet(const FObjectInitializer &ObjectInitializer);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
