// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseMissionTypeWLChecker.h"
#include "SurvivalChecker.generated.h"

/**
 * 
 */
UCLASS()
class SV_API USurvivalChecker : public UBaseMissionTypeWLChecker
{
	GENERATED_BODY()

public:

	USurvivalChecker();

	virtual UBaseMissionTypeWLChecker* ActivateChecker()override;
	
};
