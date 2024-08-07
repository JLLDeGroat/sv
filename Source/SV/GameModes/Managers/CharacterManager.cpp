// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterManager.h"
#include "../../Delegates/CharacterDelegates.h"
#include "../../Characters/BaseCharacter.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UCharacterManager::UCharacterManager(const FObjectInitializer& ObjectInitializer) : UObject(ObjectInitializer) {
}

void UCharacterManager::ReceiveNewCharacter(ABaseCharacter* character) {
	CharacterList.Emplace(character);
	UDebugMessages::LogDisplay(this, "Added: " + character->GetName() + " to character list");
}

void UCharacterManager::RemoveCharacter(FGuid Id) {
	for (int i = 0; i < CharacterList.Num(); i++) {
		if (CharacterList[i]) {
			auto actor = CharacterList[i]->GetAsActor();
			auto detailsComponent = actor->GetComponentByClass<UCharacterDetailsComponent>();

			if (!detailsComponent)
				return UDebugMessages::LogError(this, "failed to get details component, cannot remove character from manager");

			if (detailsComponent->GetCharacterId() == Id)
				return CharacterList.RemoveAt(i);
		}
	}
}

void UCharacterManager::GetCharacterListOfCharacterType(ECharacterControl characterControl, TArray<TScriptInterface<ISvChar>>& foundCharacters) {
	for (int i = 0; i < CharacterList.Num(); i++) {
		if (CharacterList[i])
			if (CharacterList[i]->IsControlType(characterControl))
				foundCharacters.Emplace(CharacterList[i]);
	}
}

TArray<TScriptInterface<ISvChar>> UCharacterManager::GetExtractedCharacters() {
	return ExtractedCharacterList;
}
void UCharacterManager::AddToExtractedCharacterList(TScriptInterface<ISvChar> svChar) {
	ExtractedCharacterList.Emplace(svChar);
}
