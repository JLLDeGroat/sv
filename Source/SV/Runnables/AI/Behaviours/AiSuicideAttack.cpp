// Fill out your copyright notice in the Description page of Project Settings.


#include "AiSuicideAttack.h"
#include "../../../Characters/Components/SuicideComponent.h"
UAiSuicideAttack::UAiSuicideAttack(const FObjectInitializer& ObjectInitializer)
	: UBaseAIBehaviour(ObjectInitializer) {

}

void UAiSuicideAttack::DoBehaviour() {
	if (CanMeleeAnyone() && GetThisEnemy()->GetComponentByClass <USuicideComponent>()) {

		// get suicide component
		// begin suicide anim
		auto suicideComp = GetThisEnemy()->GetComponentByClass <USuicideComponent>();

		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([suicideComp]
			{
				suicideComp->BeginSuicide();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
	}
	else
		SetCompletedBehaviour();
}