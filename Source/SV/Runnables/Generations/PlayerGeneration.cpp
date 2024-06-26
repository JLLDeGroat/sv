// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGeneration.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Characters/Humans/Solder.h"
#include "../../Player/GamePlayerController.h"
#include "../../Player/Components/PawnCameraComponent.h"

UBaseGenerations* UPlayerGeneration::Generate() {

	auto spawnZoneRandomized = USvUtilities::RandomizeList(StartZones);


	auto loc = spawnZoneRandomized[0];
	loc.Z += 100;
	auto world = GetWorld();

	auto controller = GetWorld()->GetFirstPlayerController<AGamePlayerController>();
	if (!controller) {
		UDebugMessages::LogError(this, "failed to get game controller, not spawning player");
		return this;
	}

	auto cameraComponent = controller->GetPawn()->GetComponentByClass<UPawnCameraComponent>();

	if (!cameraComponent) {
		UDebugMessages::LogError(this, "failed to get get camera component");
		return this;
	}

	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc, cameraComponent] {
		auto actor = world->SpawnActor<ASolder>(loc, FRotator::ZeroRotator);
		cameraComponent->UpdateCameraState(ECameraState::CS_ReTarget, loc);
		}, TStatId(), nullptr, ENamedThreads::GameThread);

	return this;
}