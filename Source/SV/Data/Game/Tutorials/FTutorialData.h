// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Enums/EConfigurationEnums.h"
#include "FTutorialData.generated.h"



USTRUCT()
struct SV_API FTutorialData
{
	GENERATED_BODY()
public:
	FTutorialData();

	bool GetCompletedTutorial(ETutorials tutorial);
	void SetCompletedTurorial(ETutorials tutorial);

protected:

	UPROPERTY() TMap< ETutorials, bool> TutorialsComplete;

};
