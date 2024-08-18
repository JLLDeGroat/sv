// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "PickupAction.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UPickupAction : public UBaseActionComponent
{
	GENERATED_BODY()
public:

	UPickupAction(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	void DoAction();
};
