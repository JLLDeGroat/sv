// Fill out your copyright notice in the Description page of Project Settings.

#include "FogGenReceiveComponent.h"
#include "../../Runnables/PostMovementRunnable.h"
#include "../../Runnables/Base/BaseRunnable.h"
#include "../../Delegates/CharacterDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UFogGenReceiveComponent::UFogGenReceiveComponent(const FObjectInitializer &ObjectInitializer)
	: UActorComponent(ObjectInitializer)
{
	auto characterDelegates = UCharacterDelegates::GetInstance();

	if (characterDelegates)
		characterDelegates->_OnFogGenerationComplete.AddDynamic(this, &UFogGenReceiveComponent::OnFogGenerationComplete);
	else
		UDebugMessages::LogDisplay(this, "Failed to get Character Delegates");
}

void UFogGenReceiveComponent::OnFogGenerationComplete()
{
	if (GetOwner() && GetOwner()->GetWorld())
		PostShootRunnable = NewObject<UPostMovementRunnable>(this)
								->InsertVariables(GetOwner())
								->Initialise(GetOwner()->GetWorld())
								->Begin();
}