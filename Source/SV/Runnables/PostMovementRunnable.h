// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseRunnable.h"
#include "PostMovementRunnable.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UPostMovementRunnable : public UBaseRunnable
{
	GENERATED_BODY()
	
public:
	virtual void ActivateThread() override;
	UPostMovementRunnable* InsertVariables(AActor* movementActor);

private:
	UPROPERTY() AActor* MovedActor;
};
