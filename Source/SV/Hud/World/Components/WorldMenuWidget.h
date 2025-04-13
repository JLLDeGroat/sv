// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldMenuWidget.generated.h"

class UButton;
/**
 *
 */
UCLASS()
class SV_API UWorldMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void WorldMapClicked();
	UFUNCTION()
	void SquadClicked();
	UFUNCTION()
	void SuppliesClicked();

	UPROPERTY(meta = (BindWidget))
	UButton *OptionsBtn;
	UPROPERTY(meta = (BindWidget))
	UButton *SquadBtn;
	UPROPERTY(meta = (BindWidget))
	UButton *ShopBtn;
	UPROPERTY(meta = (BindWidget))
	UButton *WorldMapBtn;

private:
	void OpenWorldPage(FString pageName);
	void CloseWorldPage(FString pageName);
};
