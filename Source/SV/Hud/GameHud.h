// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHud.generated.h"


class UMainGameHud;
/**
 * 
 */
UCLASS()
class SV_API AGameHud : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UMainGameHud* MainGameHud;

};
