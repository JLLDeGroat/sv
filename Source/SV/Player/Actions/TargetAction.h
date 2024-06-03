// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "TargetAction.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UTargetAction : public UBaseActionComponent
{
	GENERATED_BODY()
	
public:

	UTargetAction(const FObjectInitializer& ObjectInitializer);

	void DoAction();
};
