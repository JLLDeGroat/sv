// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "SwapWeaponAction.generated.h"

/**
 * 
 */
UCLASS()
class SV_API USwapWeaponAction : public UBaseActionComponent
{
	GENERATED_BODY()

public:

	USwapWeaponAction(const FObjectInitializer& ObjectInitializer);

	void DoAction();
	
};
