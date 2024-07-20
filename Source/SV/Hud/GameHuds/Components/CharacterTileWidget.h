// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterTileWidget.generated.h"

class UCharacterTileItemWidget;
/**
 *
 */
UCLASS()
class SV_API UCharacterTileWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	UFUNCTION() void RecieveCharacter(AActor* actor);

private:

	UCharacterTileItemWidget* CreateItemWidget();

};
