// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIBehaviour.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Interfaces/SvChar.h"
#include "../../../Delegates/AIDelegates.h"

UBaseAIBehaviour::UBaseAIBehaviour(const FObjectInitializer& ObjectInitializer)
	: UObject(ObjectInitializer) {
	CompletedBehaviour = false;

	auto aiDelegates = UAIDelegates::GetInstance();
	if (aiDelegates)
		aiDelegates->_AICharacterFinishedBehaviour.AddDynamic(this, &UBaseAIBehaviour::SetCompletedBehaviour);
	else
		UDebugMessages::LogError(this, "failed to bind to delegate");
}


void UBaseAIBehaviour::DoBehaviour() {
	UDebugMessages::LogError(this, "behaviour not coded, error");
}

void UBaseAIBehaviour::SetEnemyAndCharacters(AActor* enemy, TArray<TScriptInterface<ISvChar>> allCharacters) {
	AllCharacters = allCharacters;
	ThisEnemy = enemy;
}

void UBaseAIBehaviour::SetBehaviourTargets(TArray<TScriptInterface<ISvChar>> targets) {
	BehaviourTargets = targets;
}
void UBaseAIBehaviour::SetBehaviourTarget(TScriptInterface<ISvChar> targets) {
	BehaviourTargets.Emplace(targets);
}

AActor* UBaseAIBehaviour::GetThisEnemy() {
	return ThisEnemy;
}
TArray<TScriptInterface<ISvChar>> UBaseAIBehaviour::GetAllCharacters() {
	return AllCharacters;
}
TArray<TScriptInterface<ISvChar>> UBaseAIBehaviour::GetBehaviourTargets() {
	return BehaviourTargets;
}

bool UBaseAIBehaviour::GetCompletedBehaviour() {
	return CompletedBehaviour;
}
bool UBaseAIBehaviour::GetCompletedBehaviourAndWaitIfNot(float seconds) {
	if (!CompletedBehaviour)
		FPlatformProcess::Sleep(seconds);

	return CompletedBehaviour;
}
void UBaseAIBehaviour::SetCompletedBehaviour() {
	CompletedBehaviour = true;
	UDebugMessages::LogDisplay(this, "Behaviour set to complete");
	auto aiDelegates = UAIDelegates::GetInstance();
	if (aiDelegates)
		aiDelegates->_AICharacterFinishedBehaviour.RemoveDynamic(this, &UBaseAIBehaviour::SetCompletedBehaviour);
}