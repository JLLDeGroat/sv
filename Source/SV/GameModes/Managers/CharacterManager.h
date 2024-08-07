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

	void GetCharacterListOfCharacterType(ECharacterControl characterControl, TArray<TScriptInterface<ISvChar>>& foundCharacters);
	void ReceiveNewCharacter(ABaseCharacter* character);
	void RemoveCharacter(FGuid Id);

	TArray<TScriptInterface<ISvChar>> GetExtractedCharacters();
	void AddToExtractedCharacterList(TScriptInterface<ISvChar> svChar);

protected:

private:

	TArray<TScriptInterface<ISvChar>> CharacterList;
	TArray<TScriptInterface<ISvChar>> ExtractedCharacterList;
};
