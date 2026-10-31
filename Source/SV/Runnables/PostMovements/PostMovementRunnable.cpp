// Fill out your copyright notice in the Description page of Project Settings.

#include "PostMovementRunnable.h"
#include "../../Characters/Components/AIComponent.h"
#include "../../Delegates/AIDelegates.h"

#include "../../Enums/ECharacterEnums.h"

#include "PostMovementCrouch.h"
#include "PostMovementWinLossChecker.h"
#include "PostMovementFogHandler.h"
#include "Interfaces/PostMovementComponent.h"

#include "../../Player/Components/CameraFollowComponent.h"
#include "../../Player/Components/PawnCameraComponent.h"

#pragma optimize("", off)
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

	auto pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto cameraFollowComponent = pawn->GetComponentByClass<UCameraFollowComponent>();
	auto cameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();

	if (MovedActor->GetComponentByClass<UAIComponent>())
	{
		auto aiDelegates = UAIDelegates::GetInstance();
		if (!aiDelegates)
			return UDebugMessages::LogError(this, "failed to get ai delegates");

		if (cameraComponent->GetCurrentCameraState() == ECameraState::CS_AiShoot)
			cameraComponent->UpdateCameraState(ECameraState::CS_Control);

		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
			[aiDelegates]
			{
				aiDelegates->_AICharacterFinishedBehaviour.Broadcast();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
	}

	if (cameraFollowComponent)
		cameraFollowComponent->CompleteFollow();
}
#pragma optimize("", on)

UPostMovementRunnable *UPostMovementRunnable::InsertVariables(AActor *movementActor)
{
	MovedActor = movementActor;
	return this;
}