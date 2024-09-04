// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseMissionTypeWLChecker.generated.h"

class ISvChar;
/**
 *
 */
UCLASS()
class SV_API UBaseMissionTypeWLChecker : public UObject
{
	GENERATED_BODY()

public:

	virtual UBaseMissionTypeWLChecker* ActivateChecker();

protected:

	bool GetHasAllCharactersLeftTheField();
	bool GetHasExtractedAnyCharacters();
	
	TArray<TScriptInterface<ISvChar>> GetAllCharactersLeftOnTheField();

};
