// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseControllerManager.generated.h"

class AGamePlayerController;

/**
 * 
 */
UCLASS()
class SV_API UBaseControllerManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UBaseControllerManager(const FObjectInitializer& ObjectInitializer);

	void SetOwningController(AGamePlayerController* controller);

protected:

	UPROPERTY() AGamePlayerController* OwningController;
	
};
