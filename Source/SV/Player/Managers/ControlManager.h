// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseControllerManager.h"
#include "ControlManager.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UControlManager : public UBaseControllerManager
{
	GENERATED_BODY()

public:

	UControlManager(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE bool GetCanMouseDesignateSelectionDecal();
	void SetCanMouseDesignateSelectionDecal(bool val);

private:

	UPROPERTY() bool bCanMouseDesignateSelectionDecal;
	
};
