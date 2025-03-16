// Fill out your copyright notice in the Description page of Project Settings.

#include "PostMovementRunnable.h"
#include "../Characters/Components/AIComponent.h"
#include "../Delegates/AIDelegates.h"

#include "PostMovements/PostMovementCrouch.h"
#include "PostMovements/PostMovementWinLossChecker.h"
#include "PostMovements/PostMovementFogHandler.h"
#include "PostMovements/Interfaces/PostMovementComponent.h"

void UPostMovementRunnable::ActivateThread()
{
	auto postMovementCrouch = (IPostMovementComponent *)NewObject<UPostMovementCrouch>(this);
	postMovementCrouch->Setup(MovedActor);
	postMovementCrouch->BeginComponent();
	postMovementCrouch->Finish();

	auto postMovementWinLossChecker = (IPostMovementComponent *)NewObject<UPostMovementWinLossChecker>(this);
	postMovementWinLossChecker->Setup(MovedActor);
	postMovementWinLossChecker->BeginComponent();
	postMovementWinLossChecker->Finish();

	auto postMovementFogHandler = (IPostMovementComponent *)NewObject<UPostMovementFogHandler>(this);
	postMovementFogHandler->Setup(MovedActor);
	postMovementFogHandler->BeginComponent();
	postMovementFogHandler->Finish();

	if (MovedActor->GetComponentByClass<UAIComponent>())
	{
		auto aiDelegates = UAIDelegates::GetInstance();
		if (!aiDelegates)
			return UDebugMessages::LogError(this, "failed to get ai delegates");

		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
			[aiDelegates]
			{
				aiDelegates->_AICharacterFinishedBehaviour.Broadcast();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

UPostMovementRunnable *UPostMovementRunnable::InsertVariables(AActor *movementActor)
{
	MovedActor = movementActor;
	return this;
}