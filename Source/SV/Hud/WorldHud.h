// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WorldHud.generated.h"

class UMainWorldWidget;
/**
 * 
 */
UCLASS()
class SV_API AWorldHud : public AHUD
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;

private:

	UPROPERTY() UMainWorldWidget* MainWorldWidget;
};
