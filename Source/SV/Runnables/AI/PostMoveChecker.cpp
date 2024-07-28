// Fill out your copyright notice in the Description page of Project Settings.


#include "PostMoveChecker.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Interfaces/SvChar.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Behaviours/AIMeleeAttack.h"

#pragma optimize("", off)
void UPostMoveChecker::ActivateThread() {
	Super::ActivateThread();

	UDebugMessages::LogDisplay(this, "beginning Post Move");

	auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetAsActor()->GetActorLocation());
	auto closestCharacter = GetAllCharacters()[0];

	//check if already next to player
	if (USvUtilities::AreGridLocationsAdjacent(
		UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetAsActor()->GetActorLocation()),
		UGridUtilities::GetNormalisedGridLocation(closestCharacter->GetAsActor()->GetActorLocation())))
	{
		UDebugMessages::LogDisplay(this, "Is now Adjacent after movement, attempt to attack");

		auto meleeBehaviour = CreateBehaviourClass(UAIMeleeAttack::StaticClass());
		if (!meleeBehaviour)
			return UDebugMessages::LogError(this, "failed to get melee behaviour class");

		meleeBehaviour->SetBehaviourTarget(closestCharacter);
		meleeBehaviour->DoBehaviour();

		while (!meleeBehaviour->GetCompletedBehaviourAndWaitIfNot(.1f) && bIsAlive) {
			UDebugMessages::LogDisplay(this, "waiting on melee behaviour");
		}
	}

	SetCheckerHasCompleted();
	UDebugMessages::LogDisplay(this, "Ending Post Move");
}
#pragma optimize("", on)