// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Data/FGameData.h"
#include "CrewDataManager.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UCrewDataManager : public UObject
{
	GENERATED_BODY()

public:

	void SetCrewMembers(TArray<FCrewMember*> crewMembers);
	void AddCrewMember(FCrewMember* crewMember);

protected:

	UPROPERTY() TArray<FCrewMember> CrewMembers;
};
