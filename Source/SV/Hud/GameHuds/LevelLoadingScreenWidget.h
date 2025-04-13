// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelLoadingScreenWidget.generated.h"

class UTextBlock;
class UProgressBar;
/**
 *
 */
UCLASS()
class SV_API ULevelLoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
UFUNCTION() void OnLevelLoadingUpdate(FString Msg, float Percentage);

	UPROPERTY(meta = (BindWidget))
	UTextBlock *LoadingText;
	UPROPERTY(meta = (BindWidget))
	UProgressBar *LoadingProgress;
};
