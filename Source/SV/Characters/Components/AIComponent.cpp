// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponent.h"
#include "../../Delegates/AIDelegates.h"

#include "VgCore/Domain/Debug/DebugMessages.h"
// Sets default values for this component's properties
UAIComponent::UAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UAIComponent::BroadcastAiCompletedTask() {
	auto aiDelegates = UAIDelegates::GetInstance();
	
	if (!aiDelegates) 
		return UDebugMessages::LogError(this, "failed to get ai Delegates");

	aiDelegates->_AICharacterFinishedBehaviour.Broadcast();
}