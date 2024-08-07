// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "ExtractionAction.generated.h"

class UBaseRunnable;
/**
 * 
 */
UCLASS()
class SV_API UExtractionAction : public UBaseActionComponent
{
	GENERATED_BODY()
	
public:

	UExtractionAction(const FObjectInitializer& ObjectInitializer);
	void DoAction();

private:

	UPROPERTY() UBaseRunnable* BaseRunnable;
};
