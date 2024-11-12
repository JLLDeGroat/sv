// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "SuicideComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API USuicideComponent :public UAnimAccessComponent
{
	GENERATED_BODY()

public:
	USuicideComponent(const FObjectInitializer& ObjectInitializer);

	void BeginSuicide();
	void ActivateSuicide();
protected:

	FTimerHandle ActivationDelay;
	UFUNCTION()void ActivationDelayCallback();
};
