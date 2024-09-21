// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterTileItemWidget.generated.h"

class UWidgetAnimation;
class UButton;
class UImage; 

UCLASS()
class SV_API UCharacterTileItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	void SetRepresentedActor(AActor* actor);

	AActor* GetRepresentedActor();

	UFUNCTION() void SimulateOnCharacterButtonClicked(AActor* actor);

	void UpdateCameraCallback();
protected:

	UFUNCTION() void OnCharacterButtonClicked();
	UFUNCTION() void OnHovered();
	UFUNCTION() void OnUnhovered();
	UPROPERTY() AActor* RepresentedActor;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim)) UWidgetAnimation* AnimateIn;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim)) UWidgetAnimation* AnimateOut;

	UFUNCTION() void Deactivate();

	UPROPERTY(meta = (BindWidget)) UButton* CharacterButton;
	UPROPERTY(meta = (BindWidget)) UImage* CharacterImage;

private:

	FTimerHandle UpdateCameraHandle;
	UPROPERTY() UTexture2D* CurrentCameraTexture;

	UPROPERTY() bool Activating;
	UPROPERTY() bool Activated;

};
