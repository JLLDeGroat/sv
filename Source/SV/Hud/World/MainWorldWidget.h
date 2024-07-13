// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWorldWidget.generated.h"

/**
 *
 */
UCLASS()
class SV_API UMainWorldWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct()override;

};
