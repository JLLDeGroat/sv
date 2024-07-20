// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterDetailsWidget.generated.h"

class UProgressBar;
class UImage;
class UTextBlock;
/**
 *
 */
UCLASS()
class SV_API UCharacterDetailsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:

	UFUNCTION() void ReceiveActor(AActor* actor);
	UFUNCTION() void RefreshReceivedActor();
	UFUNCTION() void HideOrReset();
	UPROPERTY() AActor* CurrentActor;

private:

	void UpdateUIWithActor();

	UImage* GetCurrentWeaponImage();
	UProgressBar* GetAmmoProgress();
	UProgressBar* GetHealthProgress();
	UTextBlock* GetAPMPTextBlock();
};
