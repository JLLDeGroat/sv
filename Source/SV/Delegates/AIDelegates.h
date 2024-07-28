// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIDelegates.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAICharacterFinishedTurn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAICharacterFinishedBehaviour);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAiTurnIndicatorVisibility, bool, bVisible);
/**
 * 
 */
UCLASS()
class SV_API UAIDelegates : public UObject
{
	GENERATED_BODY()
public:

	static UAIDelegates* GetInstance() {
		static UAIDelegates* aiDelegates = 0; if (!aiDelegates)
		{
			aiDelegates = NewObject<UAIDelegates>();
			aiDelegates->AddToRoot();
		}
		return aiDelegates;
	}

	FAICharacterFinishedTurn _AICharacterFinishedTurn;
	FAICharacterFinishedBehaviour _AICharacterFinishedBehaviour;

	FAiTurnIndicatorVisibility _AiTurnIndicatorVisibility;

};
