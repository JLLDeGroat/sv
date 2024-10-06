// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Enums/EConfigurationEnums.h"
#include "TutorialDelegates.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTryShowTutorial, ETutorials, Tutorial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteTutorial, ETutorials, Tutorial);

UCLASS()
class SV_API UTutorialDelegates : public UObject
{
	GENERATED_BODY()
public:
	static UTutorialDelegates* GetInstance() {
		static UTutorialDelegates* tutDelegates = 0; if (!tutDelegates)
		{
			tutDelegates = NewObject<UTutorialDelegates>();
			tutDelegates->AddToRoot();
		}
		return tutDelegates;
	}

	FOnTryShowTutorial _OnTryShowTutorial;
	FOnCompleteTutorial _OnCompleteTutorial;
};
