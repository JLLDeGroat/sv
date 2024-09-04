// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMissionTypeWLChecker.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../Utilities/SvUtilities.h"
#include "../../../../Interfaces/SvChar.h"
#include "../../../../GameModes/Managers/CharacterManager.h"

UBaseMissionTypeWLChecker* UBaseMissionTypeWLChecker::ActivateChecker() {
	UDebugMessages::LogError(this, "Checker not overriding method");
	return this;
}

TArray<TScriptInterface<ISvChar>> UBaseMissionTypeWLChecker::GetAllCharactersLeftOnTheField() {
	auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());
	TArray<TScriptInterface<ISvChar>> svChars;
	characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_Player, svChars);
	return svChars;
}


bool UBaseMissionTypeWLChecker::GetHasAllCharactersLeftTheField() {
	return GetAllCharactersLeftOnTheField().IsEmpty();
}

bool UBaseMissionTypeWLChecker::GetHasExtractedAnyCharacters() {
	auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());
	return !characterManager->GetExtractedCharacters().IsEmpty();
}