// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGeneration.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Instance/Managers/CurrentGameDataManager.h"
#include "../../Instance/SvGameInstance.h"
#include "../../Characters/Humans/Solder.h"
#include "../../Player/GamePlayerController.h"
#include "../../Player/Components/PawnCameraComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/EquipmentComponent.h"
#include "../../Characters/Components/ThrowableComponent.h"

UBaseGenerations* UPlayerGeneration::Generate() {

	auto spawnZoneRandomized = USvUtilities::RandomizeList(StartZones);
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

	auto instance = USvUtilities::GetGameInstance(GetWorld());
	auto currentGameDataManager = instance->GetCurrentGameDataManager();
	auto currentGameData = currentGameDataManager->GetCurrentGameData();

	auto crew = currentGameData->GetCrew();

	for (int i = 0; i < spawnZoneRandomized.Num(); i++) {
		if (crew.Num() > 0 && (crew.Num() - 1) >= i) {

			auto loc = spawnZoneRandomized[i] + FVector(0, 0, 100);
			auto playerGen = this;

			auto thisPrimary = currentGameData->GetCrewPrimary(crew[i].GetId());
			auto gunType = thisPrimary.GetPrimaryGunType();
			auto equipment = currentGameData->GetToolsFromMember(crew[i].GetId());
			auto crewName = crew[i].GetName();

			auto self = this;

			FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc, cameraComponent, playerGen, gunType, equipment, self, crewName] {
				auto actor = world->SpawnActor<ASolder>(loc, FRotator::ZeroRotator);

				if (!actor)
					return UDebugMessages::LogError(playerGen, "failed to spawn one of thhe crew, game breaking");

				//equip primary weapon
				if (gunType != EGun::INVALID) {
					auto equipmentComponent = actor->GetComponentByClass<UEquipmentComponent>();
					equipmentComponent->EquipPrimary(gunType);
				}

				//Get throwable component
				auto throwableEquipment = actor->GetComponentByClass<UThrowableComponent>();
				if (!throwableEquipment) {
					UDebugMessages::LogError(self, "no throwable component, cannot equipment tools, stopping here");
					return;
				}

				// loop equpiment and assign (currently only assigning throwables)
				for (int i = 0; i < equipment.Num(); i++) {
					if (equipment[i]->GetToolType() == EToolType::INVALID ||
						equipment[i]->GetToolType() == EToolType::TT_HealthKit)
					{
						UDebugMessages::LogError(self, "there was an invalid tool or healthkit tool type, this aint going to work");
					}

					else if (equipment[i]->GetToolType() == EToolType::TT_Throwable)
						throwableEquipment->AddThrowable((EThrowable)equipment[i]->GetTool(), 1);
				}

				auto characterDetailsComponent = actor->GetComponentByClass<UCharacterDetailsComponent>();
				if (!characterDetailsComponent) {
					UDebugMessages::LogError(self, "failed to get characterDetails component, stopping here");
					return;
				}

				characterDetailsComponent->SetCharacterName(crewName);
				//updating camera state to look at this one
				cameraComponent->UpdateCameraState(ECameraState::CS_ReTarget, loc);

				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else
			break;
	}

	return this;
}