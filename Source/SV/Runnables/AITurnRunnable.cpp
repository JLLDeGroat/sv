// Fill out your copyright notice in the Description page of Project Settings.


#include "AITurnRunnable.h"
#include "../GameModes/GameplayMode.h"
#include "../Interfaces/Gameplay.h"
#include "../GameModes/Managers/CharacterManager.h"
#include "../Interfaces/SvChar.h"
#include "../Utilities/GridUtilities.h"
#include "../Utilities/SvUtilities.h"
#include "../Characters/Components/GridMovementComponent.h"
#include "../Characters/Components/CharacterDetailsComponent.h"
#include "../Characters/Components/EquipmentComponent.h"
#include "../Characters/Components/SkillsComponent.h"
#include "../Characters/Components/AttackComponent.h"
#include "../Equipment/Components/EquipmentDetailsComponent.h"
#include "../Equipment/Equipment.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

void UAITurnRunnable::ActivateThread() {
	UDebugMessages::LogDisplay(this, "Starting AI turns");
	TScriptInterface<IGameplay> gamePlay = GetWorld()->GetAuthGameMode<AGameplayMode>();

	auto characterManager = gamePlay->GetCharacterManager();

	TArray<TScriptInterface<ISvChar>> aiEnemies;
	characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_AI, aiEnemies);

	TArray<TScriptInterface<ISvChar>> playerCharacters;
	characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_Player, playerCharacters);


	//for each enemy
		//find closest character
		//find the route to that character
		//find movement to that character based on movement points

	// test to see any enemies that are in the same location
		//if so, get the list of same location enemies
		//steadily regress them to use the step before

	for (int i = 0; i < aiEnemies.Num(); i++) {
		auto closestCharacterToEnemy = ClosestCharactersToThisEnemy(aiEnemies[i], playerCharacters);
		if (!closestCharacterToEnemy.IsEmpty()) {
			for (int x = 0; x < closestCharacterToEnemy.Num(); x++) {

				auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(aiEnemies[i]->GetAsActor()->GetActorLocation());

				//check if already next to player
				if (USvUtilities::AreGridLocationsAdjacent(
					UGridUtilities::GetNormalisedGridLocation(aiEnemies[i]->GetAsActor()->GetActorLocation()),
					UGridUtilities::GetNormalisedGridLocation(closestCharacterToEnemy[x]->GetAsActor()->GetActorLocation())))
				{
					UDebugMessages::LogDisplay(this, "Already Adjacent, attempt to attack");
					TryMeleeAttack(aiEnemies[i], closestCharacterToEnemy[x]);
					break;
				}

				TArray<FVector> possibleDestinationLocs;
				USvUtilities::GetAdjacentGridTiles(UGridUtilities::GetNormalisedGridLocation(closestCharacterToEnemy[x]->GetAsActor()->GetActorLocation()), possibleDestinationLocs);

				possibleDestinationLocs = USvUtilities::OrderByClosestTo(sourceLoc, possibleDestinationLocs);

				for (int k = 0; k < possibleDestinationLocs.Num(); k++) {
					if (AttemptToRouteToPossibleLocation(aiEnemies[i], possibleDestinationLocs[k])) {
						break;
					}
				}
			}
		}
	}


	//move all enemies

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([gamePlay]
		{
			gamePlay->BeginPlayerTurn();
		},
		TStatId(), nullptr, ENamedThreads::GameThread);
}

TArray<TScriptInterface<ISvChar>> UAITurnRunnable::ClosestCharactersToThisEnemy(TScriptInterface<ISvChar> enemy, TArray<TScriptInterface<ISvChar>> characters) {
	auto sourceLocation = UGridUtilities::GetNormalisedGridLocation(enemy->GetAsActor()->GetActorLocation());

	auto currentCharacters = characters;

	TArray<TScriptInterface<ISvChar>> sortedCharacters;

	while (currentCharacters.Num() > 0) {

		TScriptInterface<ISvChar> closestCharacter;
		float distance = -1;
		int currentCharactersIndex = 0;

		for (int i = 0; i < currentCharacters.Num(); i++) {
			if (characters[i]) {
				auto characterLocation = UGridUtilities::GetNormalisedGridLocation(characters[i]->GetAsActor()->GetActorLocation());
				auto thisCharacterDistance = FVector::Dist(characterLocation, sourceLocation);

				if (distance < 0) {
					closestCharacter = characters[i];
					distance = thisCharacterDistance;
				}
				else {
					if (thisCharacterDistance < distance) {
						currentCharactersIndex = i;
						closestCharacter = characters[i];
						distance = thisCharacterDistance;
					}
				}
			}
		}

		sortedCharacters.Emplace(closestCharacter);
		currentCharacters.RemoveAt(currentCharactersIndex);
	}

	return sortedCharacters;
}

bool UAITurnRunnable::IsAlreadyAdjacentCharacter(FVector source, FVector characterLoc) {
	auto griddedSource = UGridUtilities::GetNormalisedGridLocation(source);
	auto griddedCharacter = UGridUtilities::GetNormalisedGridLocation(characterLoc);

	TArray<FVector> sourceAdjacents;
	USvUtilities::GetAdjacentGridTiles(griddedSource, sourceAdjacents);


	for (int i = 0; i < sourceAdjacents.Num(); i++) {
		if (griddedCharacter == sourceAdjacents[i]) return true;
	}
	return false;
}

bool UAITurnRunnable::AttemptToRouteToPossibleLocation(TScriptInterface<ISvChar> character, FVector possibleLocation) {

	auto gridMovementComponent = character->GetAsActor()->GetComponentByClass<UGridMovementComponent>();
	if (!gridMovementComponent) {
		UDebugMessages::LogError(this, "Failed to get movement component, AI cannot move");
		return false;
	}

	auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(character->GetAsActor()->GetActorLocation());
	auto movementDetails = gridMovementComponent->FindRoute(sourceLoc, possibleLocation, true);

	TArray<FVector> finalMovement;
	if (!movementDetails.IsEmpty()) {
		auto thisEnemyDetails = character->GetAsActor()->GetComponentByClass<UCharacterDetailsComponent>();

		if (movementDetails.Num() < thisEnemyDetails->GetMovementPoints()) {
			finalMovement = movementDetails;
		}
		else {
			for (int j = 0; j < thisEnemyDetails->GetMovementPoints(); j++)
				finalMovement.Emplace(movementDetails[j]);
		}
	}

	if (!finalMovement.IsEmpty()) {
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([finalMovement, gridMovementComponent]
			{
				gridMovementComponent->MoveAcrossGrid(finalMovement);
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
		return true;
	}

	return false;
}

void UAITurnRunnable::TryMeleeAttack(TScriptInterface<ISvChar> meleeAttacker, TScriptInterface<ISvChar> character) {
	auto equipmentComponent = meleeAttacker->GetAsActor()->GetComponentByClass<UEquipmentComponent>();
	auto currentAttackerDetails = meleeAttacker->GetAsActor()->GetComponentByClass<UCharacterDetailsComponent>();

	if (!equipmentComponent || !currentAttackerDetails) return
		UDebugMessages::LogError(this, "could not find equipmentComponent of currentAttack details of ai/character, will stop trying to melee");

	AEquipment* bestEquipment = nullptr;
	int bestEquipmentAttack = 0;
	if (equipmentComponent) {
		TArray<AEquipment*> meleeEquipment = equipmentComponent->GetAllMeleeEquipment();

		for (int i = 0; i < meleeEquipment.Num(); i++) {
			auto equipmentDetails = meleeEquipment[i]->GetComponentByClass<UEquipmentDetailsComponent>();
			if (!equipmentDetails) {
				UDebugMessages::LogError(this, "no equipment details found for " + meleeEquipment[i]->GetName());
				continue;
			}

			if (!bestEquipment && currentAttackerDetails->GetActionPoints() >= equipmentDetails->GetApCost()) {
				bestEquipment = meleeEquipment[i];
				bestEquipmentAttack = equipmentDetails->GetBaseDamage();
				continue;
			}

			if (bestEquipment && currentAttackerDetails->GetActionPoints() >= equipmentDetails->GetApCost()) {
				if (equipmentDetails->GetBaseDamage() > bestEquipmentAttack) {
					bestEquipment = meleeEquipment[i];
					bestEquipmentAttack = equipmentDetails->GetBaseDamage();
					continue;
				}
			}
		}
	}

	int bestSkillAttack = 0;
	FSkillDataItem* bestSkill = nullptr;
	auto skillComponent = meleeAttacker->GetAsActor()->GetComponentByClass<USkillsComponent>();
	if (skillComponent) {
		auto skills = skillComponent->GetMeleeSkills();

		for (int i = 0; i < skills.Num(); i++) {
			if (!bestSkill && currentAttackerDetails->GetActionPoints() >= skills[i]->GetApCost()) {
				bestSkillAttack = skills[i]->GetDamageMin();
				bestSkill = skills[i];
				continue;
			}

			if (!bestSkill && currentAttackerDetails->GetActionPoints() >= skills[i]->GetApCost()) {
				bestSkill = skills[i];
				bestSkillAttack = skills[i]->GetDamageMin();
				continue;
			}

			if (bestSkill && currentAttackerDetails->GetActionPoints() >= skills[i]->GetApCost()) {
				if (skills[i]->GetDamageMin() > bestSkillAttack) {
					bestSkill = skills[i];
					bestSkillAttack = skills[i]->GetDamageMin();
					continue;
				}
			}
		}
	}

	auto attackComponent = meleeAttacker->GetAsActor()->GetComponentByClass<UAttackComponent>();

	if (!attackComponent)
		return UDebugMessages::LogError(this, "could not find attack component, will not attack");

	if (bestSkill && bestEquipment) {
		if (bestEquipmentAttack > bestSkillAttack) {
			//do equipment attack
			//TODO
			UDebugMessages::LogError(this, "NOT IMPLEMENTED YET: Equipment based ai attack");
		}
		else {
			skillComponent->SetCurrentActiveSkill(bestSkill);
		}
	}
	else if (bestSkill) {
		skillComponent->SetCurrentActiveSkill(bestSkill);
	}
	else if (bestEquipment) {
		//do equipment attack
		//TODO
		UDebugMessages::LogError(this, "NOT IMPLEMENTED YET: Equipment based ai attack");
	}
	else return UDebugMessages::LogError(this, "no skill or equipment found, will not attack");

	auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(meleeAttacker->GetAsActor()->GetActorLocation());

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([attackComponent, sourceLoc, character]
		{
			attackComponent->TryAttackTarget(sourceLoc, character, false);
		},
		TStatId(), nullptr, ENamedThreads::GameThread);
}