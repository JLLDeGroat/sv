// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "ActivateToggleAction.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UActivateToggleAction : public UBaseActionComponent
{
	GENERATED_BODY()

public:

	UActivateToggleAction(const FObjectInitializer& ObjectInitializer);

	void DoAction();
	
};
