// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "SleepAction.generated.h"

/**
 * 
 */
UCLASS()
class SV_API USleepAction : public UBaseActionComponent
{
	GENERATED_BODY()

public:

	USleepAction(const FObjectInitializer& ObjectInitializer);
	
	void DoAction();
};
