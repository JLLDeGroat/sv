// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterTileUIComponent.h"
#include "../../../Delegates/HudDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
// Sets default values for this component's properties
UCharacterTileUIComponent::UCharacterTileUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCharacterTileUIComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(BroadCastDelay, this, &UCharacterTileUIComponent::BroadCastDelayCallback, 1.0f, false);
}

void UCharacterTileUIComponent::BroadCastDelayCallback() {
	auto uiDelegates = UHudDelegates::GetInstance();

	if (!uiDelegates)
		return UDebugMessages::LogError(this, "failed to get ui Delegates");

	if (uiDelegates->_AddSoldierToCharacterTileWidget.IsBound())
		uiDelegates->_AddSoldierToCharacterTileWidget.Broadcast(GetOwner());
	else
		GetWorld()->GetTimerManager().SetTimer(BroadCastDelay, this, &UCharacterTileUIComponent::BroadCastDelayCallback, 2.0f, false);
}

