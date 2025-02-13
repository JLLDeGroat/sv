// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHud.generated.h"

class UMainMenuLayoutWidget;

/**
 * 
 */
UCLASS()
class SV_API AMenuHud : public AHUD
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	

protected:

	UPROPERTY() UMainMenuLayoutWidget* MenuLayoutWidget;

};
