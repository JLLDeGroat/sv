// Fill out your copyright notice in the Description page of Project Settings.


#include "AiTurnChecker.h"
#include "../../Delegates/AIDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UAiTurnChecker::UAiTurnChecker(const FObjectInitializer& ObjectInitializer) :
	UBaseAITurnManager(ObjectInitializer) {

	auto aiDelegates = UAIDelegates::GetInstance();
	if (!aiDelegates)
		UDebugMessages::LogError(this, "failed to get aiDelegates");
	else {
		aiDelegates->_AICharacterFinishedTurn.AddDynamic(this, &UAiTurnChecker::OnCharacterFinishedTurn);
	}
}

void UAiTurnChecker::OnCharacterFinishedTurn() {
	bCanProceedToNextAiUnit = true;
}

bool UAiTurnChecker::GetCanProceedAndWaitIfNot(int seconds) const {
	if (!bCanProceedToNextAiUnit) {
		FPlatformProcess::Sleep(seconds);
	}
	return bCanProceedToNextAiUnit;
}

bool UAiTurnChecker::GetCanProceedToNextAiUnit() const {
	return bCanProceedToNextAiUnit;
}

void UAiTurnChecker::SetCanProceedToNextAiUnit(bool value) {
	bCanProceedToNextAiUnit = value;
}