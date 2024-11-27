// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterDelegates.generated.h"
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFogGenerationComplete);

class ABaseCharacter;

UCLASS()
class SV_API UCharacterDelegates : public UObject
{
	GENERATED_BODY()
public:
	static UCharacterDelegates* GetInstance() {
		static UCharacterDelegates* characterDelegates = 0; if (!characterDelegates)
		{
			characterDelegates = NewObject<UCharacterDelegates>();
			characterDelegates->AddToRoot();
		}
		return characterDelegates;
	}

	FOnFogGenerationComplete _OnFogGenerationComplete;
};
