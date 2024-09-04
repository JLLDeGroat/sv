// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectivesWidget.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UObjectivesWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION() void UpdateMainObjective(FString str);
	UFUNCTION() void UpdateSecondaryObjective(FString str);
	UFUNCTION() void UpdateTertiaryObjective(FString str);

	virtual void BeginDestroy() override;

private:

	void UpdateTextItem(FString str, FString textName);
};
