// Fill out your copyright notice in the Description page of Project Settings.

#include "ExteriorGenerations.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Environment/Constructions/Fence.h"
#include "../../Environment/Natural/SoilPlot.h"
#include "../../Environment/Prefabs/Managers/PrefabManager.h"
#include "../../Environment/Prefabs/BuildingPrefab.h"
#include "../../Environment/Prefabs/ExteriorPrefab.h"
#include "../../World/WorldGridItemActor.h"
#include "../../Utilities/SvUtilities.h"

UExteriorGenerations *UExteriorGenerations::SetAmountToGenerate(int amount)
{
	AmountToGenerate = amount;
	return this;
}
UBaseGenerations *UExteriorGenerations::Generate()
{
	FString blueprintLocation = GeneratePrefabTemplateLocations();
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

				if (AExteriorPrefab *defaultBuildingClass = Cast<AExteriorPrefab>(buildingClass->GetDefaultObject()))
					for (int j = 0; j < defaultBuildingClass->GetStartLocations().Num(); j++)
						StartLocations.Emplace(UsableLocations[i] + defaultBuildingClass->GetStartLocations()[j]);

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

FString UExteriorGenerations::GeneratePrefabTemplateLocations()
{
	auto prefabManager = NewObject<UPrefabManager>(this);
	TemplatedRequiredLocations.Empty();

	int requiredXLength, requiredYLength;
	FString blueprintLocation;
	auto randomBuildingBp = prefabManager->GetRandomExteriorPrefab(requiredXLength, requiredYLength, blueprintLocation);
	for (int i = 0; i < requiredXLength; i++)
		for (int j = 0; j < requiredYLength; j++)
			TemplatedRequiredLocations.Emplace(FVector(i * 100, j * 100, 0));

	prefabManager->ClearInternalFlags(EInternalObjectFlags::Async);
	return blueprintLocation;
}

TArray<FVector> UExteriorGenerations::GetStartLocations()
{
	return StartLocations;
}