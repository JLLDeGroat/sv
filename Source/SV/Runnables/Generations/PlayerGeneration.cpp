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
#include "../../Characters/Components/CharacterCaptureComponent.h"
#include "../../Characters/Components/HealthAndStatusWidgetComponent.h"
#include "../../Equipment/Equipment.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"

UBaseGenerations *UPlayerGeneration::Generate()
{

	auto spawnZoneRandomized = USvUtilities::RandomizeList(StartZones);
	auto world = GetWorld();

	auto controller = GetWorld()->GetFirstPlayerController<AGamePlayerController>();
	if (!controller)
	{
		UDebugMessages::LogError(this, "failed to get game controller, not spawning player");
		return this;
	}

	auto cameraComponent = controller->GetPawn()->GetComponentByClass<UPawnCameraComponent>();

	if (!cameraComponent)
	{
		UDebugMessages::LogError(this, "failed to get get camera component");
		return this;
	}

	auto instance = USvUtilities::GetGameInstance(GetWorld());
	auto currentGameDataManager = instance->GetCurrentGameDataManager();
	auto currentGameData = currentGameDataManager->GetCurrentGameData();

	auto crew = currentGameData->GetCrew();

	for (int i = 0; i < spawnZoneRandomized.Num(); i++)
	{
		if (crew.Num() > 0 && (crew.Num() - 1) >= i)
		{
			int crewNumber = i + 1;

			auto loc = spawnZoneRandomized[i] + FVector(0, 0, 100);
			auto playerGen = this;

			auto creMemberId = crew[i].GetId();

			auto thisPrimary = currentGameData->GetCrewPrimary(creMemberId);
			auto gunType = thisPrimary->GetPrimaryGunType();
			auto equipment = currentGameData->GetToolsFromMember(creMemberId);
			auto crewName = crew[i].GetName();

			auto self = this;

			FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[world, loc, cameraComponent, playerGen, gunType, equipment, self, crewName, creMemberId, thisPrimary, crewNumber]
				{
					auto actor = world->SpawnActor<ASolder>(loc, FRotator::ZeroRotator);

					if (!actor)
						return UDebugMessages::LogError(playerGen, "failed to spawn one of the crew, game breaking");

					// equip primary weapon
					if (gunType != EGun::INVALID)
					{
						auto equipmentComponent = actor->GetComponentByClass<UEquipmentComponent>();
						equipmentComponent->EquipPrimary(gunType);

						auto primaryEquipment = equipmentComponent->GetPrimaryEquipment();
						if (!primaryEquipment)
							return UDebugMessages::LogError(playerGen, "failed to get spawnedEquipment");

						auto equipmentDetailsComponent = primaryEquipment->GetComponentByClass<UEquipmentDetailsComponent>();
						equipmentDetailsComponent->SetEquipmentId(thisPrimary->GetPrimaryId());
					}

					// Get throwable component
					auto throwableEquipment = actor->GetComponentByClass<UThrowableComponent>();
					if (!throwableEquipment)
					{
						UDebugMessages::LogError(self, "no throwable component, cannot equipment tools, stopping here");
						return;
					}

					// loop equpiment and assign (currently only assigning throwables)
					for (int i = 0; i < equipment.Num(); i++)
					{
						if (equipment[i]->GetToolType() == EToolType::INVALID ||
							equipment[i]->GetToolType() == EToolType::TT_HealthKit)
						{
							UDebugMessages::LogError(self, "there was an invalid tool or healthkit tool type, this aint going to work");
						}

						else if (equipment[i]->GetToolType() == EToolType::TT_Throwable)
							throwableEquipment->AddThrowable((EThrowable)equipment[i]->GetTool(), 1, equipment[i]->GetToolId());
					}

					auto characterDetailsComponent = actor->GetComponentByClass<UCharacterDetailsComponent>();
					if (!characterDetailsComponent)
					{
						UDebugMessages::LogError(self, "failed to get characterDetails component, stopping here");
						return;
					}

					UDebugMessages::LogDisplay(self, "Gen CrewName " + crewName);

					characterDetailsComponent->SetCharacterId(creMemberId);
					characterDetailsComponent->SetCharacterName(crewName);

					auto healthAndStatusComponent = actor->GetComponentByClass<UHealthAndStatusWidgetComponent>();
					if (healthAndStatusComponent)
						healthAndStatusComponent->UpdateStatusWidgetData();

					// updating camera state to look at this one
					cameraComponent->UpdateCameraState(ECameraState::CS_ReTarget, loc);

					auto captureComponent = actor->GetComponentByClass<UCharacterCaptureComponent>();
					if (captureComponent)
						captureComponent->SetToRenderTargetNumber(crewNumber);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else
			break;
	}

	return this;
}