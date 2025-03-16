
// Fill out your copyright notice in the Description page of Project Settings.
#include "PostMovementWinLossChecker.h"
#include "../../Characters/Components/TargetingComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Delegates/HudDelegates.h"
#include "../../Delegates/GameplayDelegates.h"
#include "../../Characters/Components/ActionsComponent.h"


UPostMovementWinLossChecker::UPostMovementWinLossChecker()
{
}

void UPostMovementWinLossChecker::Setup(AActor *movedActor)
{
    MovedActor = movedActor;
}
void UPostMovementWinLossChecker::BeginComponent()
{
    auto targeting = MovedActor->GetComponentByClass<UTargetingComponent>();
	auto details = MovedActor->GetComponentByClass<UCharacterDetailsComponent>();
	if (targeting && details && details->GetCharacterControl() == ECharacterControl::CC_Player)
	{
		auto hudDelegates = UHudDelegates::GetInstance();
		if (!hudDelegates)
			return UDebugMessages::LogError(this, "failed to get hudDelegates");

		auto actionsComponent = MovedActor->GetComponentByClass<UActionsComponent>();

		if (!actionsComponent)
			return UDebugMessages::LogError(this, "failed to get actions component");

		auto gamePlayDelegates = UGameplayDelegates::GetInstance();
		if (!gamePlayDelegates)
			return UDebugMessages::LogError(this, "failed to get gameplay delegates");

		auto world = GetWorld();
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
			[hudDelegates, actionsComponent, gamePlayDelegates, targeting, world]
			{
				targeting->DetermineTargetData();
				actionsComponent->SendActionsToUI();
				hudDelegates->_RefreshCharacterDetailsWidget.Broadcast();
				gamePlayDelegates->_ChangeControlLimits.Broadcast(EControlLimit::CL_NONE);

				if (world)
					USvUtilities::AttemptToStartWinLossChecker(world); },
			TStatId(), nullptr, ENamedThreads::GameThread);
	}

}

void UPostMovementWinLossChecker::Finish()
{
    ClearInternalFlags(EInternalObjectFlags::Async);
}