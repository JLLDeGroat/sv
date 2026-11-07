// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "RepairArmourAction.generated.h"

/**
 *
 */
UCLASS()
class SV_API URepairArmourAction : public UBaseActionComponent
{
	GENERATED_BODY()

public:

	URepairArmourAction(const FObjectInitializer& ObjectInitializer);

	void DoAction();

};
