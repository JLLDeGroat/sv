// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseTutorialWidget.h"
#include "NavigatingWorldTutorialWidget.generated.h"

class UButton;
class UTextBlock;

/**
 *
 */
UCLASS()
class SV_API UNavigatingWorldTutorialWidget : public UBaseTutorialWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:


};
