// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Interfaces/UI/WorldPage.h"
#include "WorldPageWidget.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UWorldPageWidget : public UUserWidget, public IWorldPage
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void OpenPage() override;
};