// Fill out your copyright notice in the Description page of Project Settings.


#include "AiRangeMove.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UAiRangeMove::UAiRangeMove(const FObjectInitializer& ObjectInitializer)
	:UBaseAIBehaviour(ObjectInitializer) {

}

void UAiRangeMove::DoBehaviour() {
	UDebugMessages::LogError(this, "no range movement coded");
	SetCompletedBehaviour();
}