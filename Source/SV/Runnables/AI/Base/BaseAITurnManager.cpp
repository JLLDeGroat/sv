// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAITurnManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../Behaviours/AIMeleeAttack.h"
#include "../Behaviours/AIMeleeRangeMove.h"
#include "../Behaviours/AiRangeAttack.h"
#include "../Behaviours/AiRangeMove.h"
#include "../Behaviours/AiSuicideAttack.h"

#include "../../../Interfaces/SvChar.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"

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
bool UBaseAITurnManager::GetThisEnemyIsValidAndAlive() {
	if (GetThisEnemy()) {
		auto detailComponent = GetThisEnemy()->GetAsActor()->GetComponentByClass<UCharacterDetailsComponent>();
		if (!detailComponent)
			UDebugMessages::LogError(this, "failed to get detailsComponent for this enemy " + GetThisEnemy()->GetAsActor()->GetName());

		return !detailComponent->GetIsDead();
	}
	return false;
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

	if (UAIMeleeAttack::StaticClass() == cls)			behaviour = NewObject<UAIMeleeAttack>(this);
	else if (UAIMeleeRangeMove::StaticClass() == cls)	behaviour = NewObject<UAIMeleeRangeMove>(this);

	if (behaviour)
		behaviour->SetEnemyAndCharacters(ThisEnemy->GetAsActor(), AllCharacters);

	return behaviour;
}

UBaseAIBehaviour* UBaseAITurnManager::CreateBehaviourClass(EAIBehaviourMoveRoutes mRoute) {
	UBaseAIBehaviour* behaviour = nullptr;

	switch (mRoute) {
	case EAIBehaviourMoveRoutes::BMR_Range:
		behaviour = NewObject<UAiRangeMove>(this);
		break;
	case EAIBehaviourMoveRoutes::BMR_Melee:
		behaviour = NewObject<UAIMeleeRangeMove>(this);
		break;
	}

	if (behaviour)
		behaviour->SetEnemyAndCharacters(ThisEnemy->GetAsActor(), AllCharacters);

	return behaviour;
}
UBaseAIBehaviour* UBaseAITurnManager::CreateBehaviourClass(EAIBehaviourAttackRoutes aRoute) {
	UBaseAIBehaviour* behaviour = nullptr;

	switch (aRoute) {
	case EAIBehaviourAttackRoutes::BAR_Range:
		behaviour = NewObject<UAiRangeAttack>(this);
		break;
	case EAIBehaviourAttackRoutes::BAR_Melee:
		behaviour = NewObject<UAIMeleeAttack>(this);
		break;
	case EAIBehaviourAttackRoutes::BAR_Suicide:
		behaviour = NewObject<UAiSuicideAttack>(this);
		break;
	}

	if (behaviour)
		behaviour->SetEnemyAndCharacters(ThisEnemy->GetAsActor(), AllCharacters);

	return behaviour;
}