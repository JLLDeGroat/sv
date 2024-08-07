// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Mission/FMissionDetails.h"
#include "FCurrentWorldData.generated.h"



USTRUCT()
struct SV_API FWorldLocationData
{
	GENERATED_BODY()

public:
	FWorldLocationData() {
		Location = FVector2D::ZeroVector;
		bHasVisited = false;
		MissionDetails = FMissionDetails();
		Id = FGuid::NewGuid();
	}

	FWorldLocationData(FVector2D location, bool bVisited = false) {
		Location = location;
		bHasVisited = bVisited;
		MissionDetails = FMissionDetails();
		Id = FGuid::NewGuid();
	}

	FGuid GetId() { return Id; }
	FVector2D GetLocation() { return Location; }
	void SetIsCurrent(bool val) { bIsCurrent = val; }
	void SetIsOffshoot(bool val) { bIsOffshoot = val; }

	bool GetIsCurrent() { return bIsCurrent; }
	bool GetIsOffshoot() { return bIsOffshoot; }

	FMissionDetails* GetMissionDetails() { return &MissionDetails; }
protected:
	UPROPERTY() FVector2D Location;
	UPROPERTY() bool bHasVisited;
	UPROPERTY() FMissionDetails MissionDetails;
	UPROPERTY() FGuid Id;

	UPROPERTY() bool bIsCurrent;
	UPROPERTY() bool bIsOffshoot;
};



USTRUCT()
struct SV_API FCurrentWorldData
{
	GENERATED_BODY()

public:
	FCurrentWorldData();

	TArray<FWorldLocationData*> GetWorldLocationData();
	FGuid AddWorldLocationData(FVector2D location, bool bVisited = false);
	FWorldLocationData* GetWorldLocationData(FGuid id);
	FWorldLocationData* GetWorldLocationData(FVector2D location);
	TArray<FVector2D>GetTotalRoute();

	FWorldLocationData* GetCurrentLocation();

	void SetNewLocationAsCurrent(FVector2D location);

protected:

	UPROPERTY() TArray<FWorldLocationData> WorldLocationData;

};
