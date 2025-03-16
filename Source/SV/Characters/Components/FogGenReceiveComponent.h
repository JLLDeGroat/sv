// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FogGenReceiveComponent.generated.h"

class UBaseRunnable;
/**
 *
 */
UCLASS()
class SV_API UFogGenReceiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFogGenReceiveComponent(const FObjectInitializer &ObjectInitializer);

protected:
	UFUNCTION()
	void OnFogGenerationComplete();

	UPROPERTY() UBaseRunnable* PostShootRunnable;
};
