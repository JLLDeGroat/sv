// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../../Enums/EEquipmentEnums.h"
#include "ClothingHelpers.generated.h"

class AClothing;
/**
 *
 */
UCLASS()
class SV_API UClothingHelpers : public UObject
{
	GENERATED_BODY()

public:
	static UClass *DetermineClothingClass(EClothingType clothingType, EClothingGrade clothingGrade);
};
