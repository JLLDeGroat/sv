// Fill out your copyright notice in the Description page of Project Settings.


#include "AiTurnMoveChecker.h"

#include "../../Interfaces/SvChar.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Utilities/SvUtilities.h"

#include "../../Characters/Components/GridMovementComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"

#include "Behaviours/AIMeleeRangeMove.h"

void UAiTurnMoveChecker::ActivateThread() {
	Super::ActivateThread();

	UDebugMessages::LogDisplay(this, "beginning move checker");
	//TODO create behaviour aroud movement
	//currently assuming all is melee and will move into melee range

	auto meleeMovement = CreateBehaviourClass(UAIMeleeRangeMove::StaticClass());
	if (!meleeMovement)
		return UDebugMessages::LogError(this, "found no melee movement behaviour");

	meleeMovement->DoBehaviour();

	while (!meleeMovement->GetCompletedBehaviourAndWaitIfNot(.1f) && bIsAlive) {
		UDebugMessages::LogDisplay(this, "waiting on melee range movement behaviour");
	}

	UDebugMessages::LogDisplay(this, "ending move checker");
	SetCheckerHasCompleted();
}

