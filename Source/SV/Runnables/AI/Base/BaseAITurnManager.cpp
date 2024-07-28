// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAITurnManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../Behaviours/AIMeleeAttack.h"
#include "../Behaviours/AIMeleeRangeMove.h"

#include "../../../Interfaces/SvChar.h"

UBaseAITurnManager::UBaseAITurnManager(const FObjectInitializer& ObjectInitializer)
	: UBaseRunnable(ObjectInitializer) {

	bCheckerIsComplete = false;
}

UWorld* UBaseAITurnManager::GetWorld() const { return World; }

void UBaseAITurnManager::ActivateThread() {
	Super::ActivateThread();
}

void UBaseAITurnManager::SetupTurnManager(UWorld* world) {
	World = world;
}

void UBaseAITurnManager::SetThisEnemy(TScriptInterface<ISvChar> enemy) {
	ThisEnemy = enemy;
}
void UBaseAITurnManager::SetAllCharacters(TArray<TScriptInterface<ISvChar>> characters) {
	AllCharacters = characters;
}

TScriptInterface<ISvChar> UBaseAITurnManager::GetThisEnemy() {
	return ThisEnemy;
}
TArray<TScriptInterface<ISvChar>> UBaseAITurnManager::GetAllCharacters() {
	return AllCharacters;
}

bool UBaseAITurnManager::GetCheckerHasCompleted() const {
	return bCheckerIsComplete;
}
bool UBaseAITurnManager::GetCheckerHasCompletedAndWaitIfNot(int seconds) const {
	if (!bCheckerIsComplete) {
		FPlatformProcess::Sleep(seconds);
	}
	return bCheckerIsComplete;
}
void UBaseAITurnManager::SetCheckerHasCompleted() {
	bCheckerIsComplete = true;
}

void UBaseAITurnManager::SetHasFinishedTurnEarly() {
	HasFinishedTurnEarly = true;
}
bool UBaseAITurnManager::GetHasFinishedTurnEarly() {
	return HasFinishedTurnEarly;
}

UBaseAIBehaviour* UBaseAITurnManager::CreateBehaviourClass(UClass* cls) {
	UBaseAIBehaviour* behaviour = nullptr;

	if (UAIMeleeAttack::StaticClass() == cls)			behaviour = NewObject<UAIMeleeAttack>();
	else if (UAIMeleeRangeMove::StaticClass() == cls)	behaviour = NewObject<UAIMeleeRangeMove>();

	if (behaviour)
		behaviour->SetEnemyAndCharacters(ThisEnemy->GetAsActor(), AllCharacters);

	return behaviour;
}

