// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "HealthKitUseComponent.generated.h"

class AHealthKitUseIndicator;

UCLASS()
class SV_API UHealthKitUseComponent : public UBaseActionComponent
{
	GENERATED_BODY()

public:

	UHealthKitUseComponent(const FObjectInitializer& ObjectInitializer);

	void DoBasicHealth();
	void DoLargeHealth();
	void DoAntidote();
	void DoFlameRetardent();

	TArray<AActor*> FindAdjacentActors(AActor* actor);

	void RemoveAllHealthKitIndicators();

private:
	UPROPERTY() TArray<AHealthKitUseIndicator*> HealthKitsIndicators;
};
