// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseRunnable.h"
#include "AITurnRunnable.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UAITurnRunnable : public UBaseRunnable
{
	GENERATED_BODY()
public:

	virtual void ActivateThread() override;
};
