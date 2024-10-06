// Fill out your copyright notice in the Description page of Project Settings.


#include "PostMoveChecker.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Interfaces/SvChar.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Behaviours/AIMeleeAttack.h"
#include "../../Characters/Components/AIComponent.h"


void UPostMoveChecker::ActivateThread() {
	Super::ActivateThread();

	UDebugMessages::LogDisplay(this, "beginning Post Move");

	auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetAsActor()->GetActorLocation());
	auto closestCharacter = GetAllCharacters()[0];

	auto aiComponent = GetThisEnemy()->GetAsActor()->GetComponentByClass<UAIComponent>();

	auto postMoveBehaviour = CreateBehaviourClass(aiComponent->GetAttackRoute());
	if (!postMoveBehaviour)
		return UDebugMessages::LogError(this, "failed to get Post Move behaviour class");

	postMoveBehaviour->DoBehaviour();

	while (!postMoveBehaviour->GetCompletedBehaviourAndWaitIfNot(.1f) && bIsAlive) {
		UDebugMessages::LogDisplay(this, "waiting on Post Move behaviour");
	}

	////check if already next to player
	//if (USvUtilities::AreGridLocationsAdjacent(
	//	UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetAsActor()->GetActorLocation()),
	//	UGridUtilities::GetNormalisedGridLocation(closestCharacter->GetAsActor()->GetActorLocation())))
	//{
	//	UDebugMessages::LogDisplay(this, "Is now Adjacent after movement, attempt to attack");

	//	auto postMoveBehaviour = CreateBehaviourClass(aiComponent->GetAttackRoute());
	//	if (!postMoveBehaviour)
	//		return UDebugMessages::LogError(this, "failed to get melee behaviour class");

	//	postMoveBehaviour->SetBehaviourTarget(closestCharacter);
	//	postMoveBehaviour->DoBehaviour();

	//	while (!postMoveBehaviour->GetCompletedBehaviourAndWaitIfNot(.1f) && bIsAlive) {
	//		UDebugMessages::LogDisplay(this, "waiting on melee behaviour");
	//	}
	//}

	SetCheckerHasCompleted();
	UDebugMessages::LogDisplay(this, "Ending Post Move");
}
