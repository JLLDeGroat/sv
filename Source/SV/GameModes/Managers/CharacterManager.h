// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Enums/ECharacterEnums.h"
#include "CharacterManager.generated.h"

class ISvChar;
class ABaseCharacter;

/**
 * 
 */
UCLASS()
class SV_API UCharacterManager : public UObject
{
	GENERATED_BODY()

public:
	UCharacterManager(const FObjectInitializer& ObjectInitializer);
	
	void AssignDelegates();

	void GetCharacterListOfCharacterType(ECharacterControl characterControl, TArray<TScriptInterface<ISvChar>>& foundCharacters);

protected:

	UFUNCTION() void OnReceiveNewCharacter(ABaseCharacter* character);
	UFUNCTION() void OnRemoveCharacter(FGuid Id);

private:

	TArray<TScriptInterface<ISvChar>> CharacterList;
};
