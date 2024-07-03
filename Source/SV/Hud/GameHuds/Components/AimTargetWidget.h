// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AimTargetWidget.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UAimTargetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION() void SetAimTargetVisibility(bool val);
};
