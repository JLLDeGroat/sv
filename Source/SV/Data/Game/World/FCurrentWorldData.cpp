// Fill out your copyright notice in the Description page of Project Settings.


#include "FCurrentWorldData.h"

FCurrentWorldData::FCurrentWorldData() {

}

TArray<FWorldLocationData*> FCurrentWorldData::GetWorldLocationData() {
	TArray<FWorldLocationData*> result;

	for (int i = 0; i < WorldLocationData.Num(); i++)
		result.Emplace(&WorldLocationData[i]);

	return result;
}

FGuid FCurrentWorldData::AddWorldLocationData(FVector2D location, bool bVisited) {
	FWorldLocationData newDataItem = FWorldLocationData(location, bVisited);
	WorldLocationData.Emplace(newDataItem);
	return newDataItem.GetId();
}

FWorldLocationData* FCurrentWorldData::GetWorldLocationData(FGuid id) {
	for (int i = 0; i < WorldLocationData.Num(); i++)
		if (WorldLocationData[i].GetId() == id)
			return &WorldLocationData[i];

	return nullptr;
}

FWorldLocationData* FCurrentWorldData::GetWorldLocationData(FVector2D location) {
	for (int i = 0; i < WorldLocationData.Num(); i++)
		if (WorldLocationData[i].GetLocation() == location)
			return &WorldLocationData[i];

	return nullptr;
}

TArray<FVector2D> FCurrentWorldData::GetTotalRoute() {
	TArray<FVector2D> locations;
	for (int i = 0; i < WorldLocationData.Num(); i++)
		locations.Emplace(WorldLocationData[i].GetLocation());
	return locations;
}

FWorldLocationData* FCurrentWorldData::GetCurrentLocation() {
	for (int i = 0; i < WorldLocationData.Num(); i++)
		if (WorldLocationData[i].GetIsCurrent())
			return &WorldLocationData[i];
	return nullptr;
}

void FCurrentWorldData::SetNewLocationAsCurrent(FVector2D location) {
	for (int i = 0; i < WorldLocationData.Num(); i++)
		if (WorldLocationData[i].GetIsCurrent())
			WorldLocationData[i].SetIsCurrent(false);

	auto newCurrent = GetWorldLocationData(location);
	newCurrent->SetIsCurrent(true);
}