// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Enums/EEquipmentEnums.h"
#include "ClothingItem.generated.h"

/**
 *
 */
UINTERFACE()
class SV_API UClothingItem : public UInterface
{
    GENERATED_BODY()
};

class SV_API IClothingItem
{

    GENERATED_BODY()

public:
    virtual EClothingType GetClothingType() = 0;
    virtual EClothingGrade GetClothingGrade() = 0;
};