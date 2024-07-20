// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../../Characters/Components/CharacterDetailsComponent.h"

USleepAction::USleepAction(const FObjectInitializer& ObjectInitializer)
	: UBaseActionComponent(ObjectInitializer) {

}

void USleepAction::DoAction() {
	ResetActionEffects();
	UDebugMessages::LogError(this, "TODO Sleep function");
}