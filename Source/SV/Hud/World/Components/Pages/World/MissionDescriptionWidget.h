// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionDescriptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UMissionDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;


	bool GetMoveCompleted() const;

protected:

	UFUNCTION() void OnWorldMoveComplete();
	UFUNCTION() void OnStartMissionClicked();

private:

	UPROPERTY() bool MoveCompleted;
};