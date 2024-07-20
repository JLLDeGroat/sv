// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterTileItemWidget.generated.h"

class UWidgetAnimation;
/**
 *
 */
UCLASS()
class SV_API UCharacterTileItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	void SetRepresentedActor(AActor* actor);

protected:

	UFUNCTION() void OnCharacterButtonClicked();
	UFUNCTION() void SimulateOnCharacterButtonClicked(AActor* actor);
	UPROPERTY() AActor* RepresentedActor;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim)) UWidgetAnimation* AnimateIn;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim)) UWidgetAnimation* AnimateOut;

	UFUNCTION() void Deactivate();

private:

	UPROPERTY() bool Activating;
	UPROPERTY() bool Activated;

};
