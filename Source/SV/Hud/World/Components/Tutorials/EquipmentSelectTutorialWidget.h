// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseTutorialWidget.h"
#include "EquipmentSelectTutorialWidget.generated.h"

/**
 *
 */
UCLASS()
class SV_API UEquipmentSelectTutorialWidget : public UBaseTutorialWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
};
