// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "OverwatchAction.generated.h"

class AOverwatchArea;

/**
 *
 */
UCLASS()
class SV_API UOverwatchAction : public UBaseActionComponent
{
	GENERATED_BODY()

public:

	UOverwatchAction(const FObjectInitializer& ObjectInitializer);

	void DoAction();
	void SetOverwatch();

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY() AOverwatchArea* OverwatchArea;
};
