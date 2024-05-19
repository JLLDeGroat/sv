// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseControllerManager.generated.h"

class AGamePlayerController;

/**
 * 
 */
UCLASS()
class SV_API UBaseControllerManager : public UObject
{
	GENERATED_BODY()

public:

	UBaseControllerManager(const FObjectInitializer& ObjectInitializer);

	void SetOwningController(AGamePlayerController* controller);

protected:

	UPROPERTY() AGamePlayerController* OwningController;
	
};
