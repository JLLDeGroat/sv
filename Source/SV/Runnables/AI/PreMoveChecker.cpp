#include "PreMoveChecker.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Interfaces/SvChar.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Behaviours/AIMeleeAttack.h"
#include "../../Characters/Components/AIComponent.h"

#pragma optimize("", off)
void UPreMoveChecker::ActivateThread() {
	Super::ActivateThread();

	UDebugMessages::LogDisplay(this, "beginning pre Move checker");

	DetermineClosestCharactersToThisEnemy();

	auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetAsActor()->GetActorLocation());

	auto closestCharacter = GetAllCharacters()[0];

	//check if already next to player
	if (USvUtilities::AreGridLocationsAdjacent(
		UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetAsActor()->GetActorLocation()),
		UGridUtilities::GetNormalisedGridLocation(closestCharacter->GetAsActor()->GetActorLocation())))
	{
		UDebugMessages::LogDisplay(this, "Already Adjacent, attempt to attack");
		//TODO: should check if this enemy wants to melee or run to cover
		auto aiComponent = GetThisEnemy()->GetAsActor()->GetComponentByClass<UAIComponent>();

		if (!aiComponent)
			return UDebugMessages::LogError(this, "failed to get ai component");

		MeleeBehaviour = CreateBehaviourClass(aiComponent->GetAttackRoute());
		if (!MeleeBehaviour)
			return UDebugMessages::LogError(this, "failed to get melee behaviour class");

		MeleeBehaviour->SetBehaviourTarget(closestCharacter);
		MeleeBehaviour->DoBehaviour();

		while (!MeleeBehaviour->GetCompletedBehaviourAndWaitIfNot(.1f) && bIsAlive) {
			UDebugMessages::LogDisplay(this, "waiting on melee behaviour");
		}

		SetHasFinishedTurnEarly();
		// set via delegate
		// SetCheckerHasCompleted(); wit until attack is over
	}
	else {
		UDebugMessages::LogDisplay(this, "not adjacent to creatures");
		UDebugMessages::LogWarning(this, "TODO: check if enemy is range");
	}

	SetCheckerHasCompleted();
	UDebugMessages::LogDisplay(this, "ending pre Move checker");
}

void UPreMoveChecker::DetermineClosestCharactersToThisEnemy() {
	auto sourceLocation = UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetAsActor()->GetActorLocation());

	auto currentCharacters = GetAllCharacters();

	TArray<TScriptInterface<ISvChar>> sortedCharacters;

	while (currentCharacters.Num() > 0) {

		TScriptInterface<ISvChar> closestCharacter;
		float distance = -1;
		int currentCharactersIndex = 0;

		for (int i = 0; i < currentCharacters.Num(); i++) {
			if (currentCharacters[i]) {
				auto characterLocation = UGridUtilities::GetNormalisedGridLocation(currentCharacters[i]->GetAsActor()->GetActorLocation());
				auto thisCharacterDistance = FVector::Dist(characterLocation, sourceLocation);

				if (distance < 0) {
					closestCharacter = currentCharacters[i];
					distance = thisCharacterDistance;
				}
				else {
					if (thisCharacterDistance < distance) {
						currentCharactersIndex = i;
						closestCharacter = currentCharacters[i];
						distance = thisCharacterDistance;
					}
				}
			}
		}

		sortedCharacters.Emplace(closestCharacter);
		currentCharacters.RemoveAt(currentCharactersIndex);
	}

	Targets = sortedCharacters;
}

TArray<TScriptInterface<ISvChar>> UPreMoveChecker::GetCurrentTarget() {
	return Targets;
}
#pragma optimize("", on)