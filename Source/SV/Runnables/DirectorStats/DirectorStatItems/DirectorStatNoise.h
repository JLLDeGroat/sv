// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseDirectorStatUpdate.h"
#include "DirectorStatNoise.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UDirectorStatNoise : public UBaseDirectorStatUpdate
{
	GENERATED_BODY()
	
public:

	virtual void ActivateStatUpdate() override;
};
