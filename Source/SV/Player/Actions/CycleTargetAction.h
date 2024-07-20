// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "CycleTargetAction.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UCycleTargetAction : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	UCycleTargetAction(const FObjectInitializer& ObjectInitializer);

	void DoAction();
	
};
