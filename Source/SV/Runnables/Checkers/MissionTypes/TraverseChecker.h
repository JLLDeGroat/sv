// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseMissionTypeWLChecker.h"
#include "TraverseChecker.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UTraverseChecker : public UBaseMissionTypeWLChecker
{
	GENERATED_BODY()
	
public:

	UTraverseChecker();

	virtual UBaseMissionTypeWLChecker* ActivateChecker()override;
};
