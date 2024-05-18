// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterManager.h"
#include "../../Delegates/CharacterDelegates.h"
#include "../../Characters/BaseCharacter.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UCharacterManager::UCharacterManager(const FObjectInitializer& ObjectInitializer) : UObject(ObjectInitializer) {
}

void UCharacterManager::AssignDelegates() {
	auto characterDelegates = UCharacterDelegates::GetInstance();

	if (characterDelegates)
		characterDelegates->_ReceiveNewCharacter.AddDynamic(this, &UCharacterManager::OnReceiveNewCharacter);
	else
		UDebugMessages::LogError(this, "AssignDelegates failed assigning delegates");
}

void UCharacterManager::OnReceiveNewCharacter(ABaseCharacter* character) {
	CharacterList.Emplace(character);

	UDebugMessages::LogDisplay(this, "Added: " + character->GetName() + " to character list");
}

void UCharacterManager::GetCharacterListOfCharacterType(ECharacterControl characterControl, TArray<TScriptInterface<ISvChar>>& foundCharacters) {
	for (int i = 0; i < CharacterList.Num(); i++) {
		if (CharacterList[i] && CharacterList[i]->IsControlType(characterControl)) 
			foundCharacters.Emplace(CharacterList[i]);
	}
}
