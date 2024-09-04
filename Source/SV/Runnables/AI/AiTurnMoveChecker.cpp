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

	MeleeBehaviour = CreateBehaviourClass(UAIMeleeRangeMove::StaticClass());
	if (!MeleeBehaviour)
		return UDebugMessages::LogError(this, "found no melee movement behaviour");

	MeleeBehaviour->DoBehaviour();

	while (!MeleeBehaviour->GetCompletedBehaviourAndWaitIfNot(.1f) && bIsAlive) {
		if (GetThisEnemyIsValidAndAlive()) {
			UDebugMessages::LogDisplay(this, "waiting on melee range movement behaviour");
		}
		else {
			SetCheckerHasCompleted();
			return;
		}
	}

	UDebugMessages::LogDisplay(this, "ending move checker");
	SetCheckerHasCompleted();
}

