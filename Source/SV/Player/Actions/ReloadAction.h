// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "ReloadAction.generated.h"

/**
 *
 */
UCLASS()
class SV_API UReloadAction : public UBaseActionComponent
{
	GENERATED_BODY()

public:

	UReloadAction(const FObjectInitializer& ObjectInitializer);

	void DoAction();

};
