// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FGameData.generated.h"


USTRUCT()
struct FCrewMember
{
	GENERATED_BODY()
public:
	FCrewMember() {
		Id = FGuid::NewGuid();
	}

	void SetName(FString name) { Name = name; }
	void SetLastName(FString lastName) { LastName = lastName; }
	void SetBio(FString bio) { Bio = bio; }

	FString GetName() { return Name; }
	FString GetLastName() { return LastName; }
	FString GetBio() { return Bio; }
protected:

	UPROPERTY() FGuid Id;
	UPROPERTY() FString Name;
	UPROPERTY() FString LastName;
	UPROPERTY() FString Bio;
};


/**
 *
 */
USTRUCT()
struct FGameData
{
	GENERATED_BODY()
public:
	FGameData() {

	}

protected:

	UPROPERTY() TArray<FCrewMember> CrewMembers;
};

