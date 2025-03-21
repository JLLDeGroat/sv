// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Enums/ECharacterEnums.h"
#include "../Enums/EControllerEnums.h"
#include "GameplayDelegates.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActivateOverwatchActors, ECharacterControl, CharacterControl);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveUnusedOverwatchActors, ECharacterControl, CharacterControl);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeControlLimits, EControlLimit, ControllerLimit);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewLinkAdded, FVector, Start, FVector, End);

/**
 * 
 */
UCLASS()
class SV_API UGameplayDelegates : public UObject
{
	GENERATED_BODY()

public:

	static UGameplayDelegates* GetInstance() {
		static UGameplayDelegates* gameplayDelegates = 0; if (!gameplayDelegates)
		{
			gameplayDelegates = NewObject<UGameplayDelegates>();
			gameplayDelegates->AddToRoot();
		}
		return gameplayDelegates;
	}
	
	FActivateOverwatchActors _ActivateOverwatchActors;
	FRemoveUnusedOverwatchActors _RemoveUnusedOverwatchActors;

	FChangeControlLimits _ChangeControlLimits;
	FOnNewLinkAdded _OnNewLinkAdded;
};
