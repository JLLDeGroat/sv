// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseMissionTypeWLChecker.h"
#include "ExterminateChecker.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UExterminateChecker : public UBaseMissionTypeWLChecker
{
	GENERATED_BODY()
	
public:

	UExterminateChecker();

	virtual UBaseMissionTypeWLChecker* ActivateChecker()override;
};
