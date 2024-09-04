// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseMissionTypeWLChecker.h"
#include "ExtractionChecker.generated.h"

/**
 *
 */
UCLASS()
class SV_API UExtractionChecker : public UBaseMissionTypeWLChecker
{
	GENERATED_BODY()

public:

	UExtractionChecker();

	virtual UBaseMissionTypeWLChecker* ActivateChecker()override;

};
