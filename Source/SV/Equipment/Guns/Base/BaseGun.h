// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Equipment.h"
#include "BaseGun.generated.h"

class UGunFireComponent;
class UGunFireSoundComponent;

UCLASS()
class SV_API ABaseGun : public AEquipment
{
    GENERATED_BODY()

public:
    ABaseGun(const FObjectInitializer &ObjectInitializer);

    virtual void OnConstruction(const FTransform &Transform) override;

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UGunFireComponent *GunFireComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UGunFireSoundComponent *GunFireSoundComponent;
};
