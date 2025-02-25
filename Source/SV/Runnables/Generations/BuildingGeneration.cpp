// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingGeneration.h"
#include "../../World/WorldGridItemActor.h"
#include "../../Environment/Constructions/Wall.h"
#include "../../Environment/Constructions/Door.h"
#include "../../Environment/Constructions/WallWindow.h"
#include "../../Environment/Constructions/CementFloor.h"
#include "../../Environment/Prefabs/Managers/PrefabManager.h"
#include "../../Environment/Prefabs/BuildingPrefab.h"
#include "../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Utilities/LoadBpActorTask.h"

UBuildingGeneration::UBuildingGeneration()
{
	AmountToGenerate = 3;
	AmountGenerated = 0;
}
UBuildingGeneration *UBuildingGeneration::SetAmountToGenerate(int amount)
{
	AmountToGenerate = amount;
	return this;
}
TArray<FVector> UBuildingGeneration::GetDoorLocations()
{
	return DoorLocations;
}
#pragma optimize("", off)
UBaseGenerations *UBuildingGeneration::Generate()
{
	FString blueprintLocation = GeneratePrefabTemplateLocations();

	UDebugMessages::LogDisplay(this, "Generating Building: allowed spots " + FString::SanitizeFloat(UsableLocations.Num()));
	for (int i = 0; i < UsableLocations.Num(); i++)
	{
		if (CanFitInLocation(UsableLocations[i]))
		{
			if (ShouldGenerate())
			{
				AmountGenerated++;
				DecrementChance();

				auto world = GetWorld();
				auto location = UsableLocations[i];
				UObject *buildingObject = nullptr;
				LoadBlueprintActorInThread(blueprintLocation, buildingObject);
				auto buildingClass = (UClass *)buildingObject;

				if (ABuildingPrefab *defaultBuildingClass = Cast<ABuildingPrefab>(buildingClass->GetDefaultObject()))
					for (int j = 0; j < defaultBuildingClass->GetDoorLocations().Num(); j++)
						DoorLocations.Emplace(UsableLocations[i] + defaultBuildingClass->GetDoorLocations()[j]);

				FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
					[world, blueprintLocation, location, buildingClass]
					{
						world->SpawnActor<AActor>(buildingClass, location, FRotator::ZeroRotator);
					},
					TStatId(), nullptr, ENamedThreads::GameThread);

				IterationRequiredLocations = CreateRequiredLocations(UsableLocations[i]);
				RequiredBufferLocations = CombineList(RequiredBufferLocations, GenerateBufferAroundLocations(IterationRequiredLocations, 2));
				TotalUsedLocations = CombineList(CombineList(IterationRequiredLocations, RequiredLocations), RequiredBufferLocations);
				RequiredLocations = CombineList(RequiredLocations, IterationRequiredLocations);

				IterationRequiredLocations.Empty();
				blueprintLocation = GeneratePrefabTemplateLocations();
			}
			else
			{
				// reset requiredlocations for new house size
				// TemplatedRequiredLocations = GetBuildingRequiredLocations();
				IncrementChance();
			}

			if (AmountGenerated >= AmountToGenerate)
				break;
		}
	}
	return this;
}

FString UBuildingGeneration::GeneratePrefabTemplateLocations()
{
	auto prefabManager = NewObject<UPrefabManager>(this);
	TemplatedRequiredLocations.Empty();

	int requiredXLength, requiredYLength;
	FString blueprintLocation;
	auto randomBuildingBp = prefabManager->GetRandomBuildingPrefab(requiredXLength, requiredYLength, blueprintLocation);
	for (int i = 0; i < requiredXLength; i++)
		for (int j = 0; j < requiredYLength; j++)
			TemplatedRequiredLocations.Emplace(FVector(i * 100, j * 100, 0));

	prefabManager->ClearInternalFlags(EInternalObjectFlags::Async);
	return blueprintLocation;
}

#pragma optimize("", on)