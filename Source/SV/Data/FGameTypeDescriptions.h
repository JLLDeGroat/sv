// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enums/EConfigurationEnums.h"
#include "FGameTypeDescriptions.generated.h"
/**
 *
 */
USTRUCT()
struct FGameTypeDescriptionItem
{
	GENERATED_BODY()
public:
	FGameTypeDescriptionItem()
	{
		Title = "";
		GameModeType = EGameModeType::INVALID;
	}

	FString GetTitle() { return Title; }
	TArray<FString> GetModifierItems() { return ModifierItems; }
	TArray<FString> GetEquipmentItems() { return EquipmentItems; }
	EGameModeType GetGameModeType() { return GameModeType; }
	FString GetModeGoal() { return ModeGoal; }

protected:
	UPROPERTY()
	FString Title;
	UPROPERTY()
	TArray<FString> ModifierItems;
	UPROPERTY()
	TArray<FString> EquipmentItems;
	UPROPERTY()
	FString ModeGoal;
	UPROPERTY()
	EGameModeType GameModeType;
};

USTRUCT()
struct FGameTypeDescriptions
{
	GENERATED_BODY()
public:
	FGameTypeDescriptions()
	{
	}

	FGameTypeDescriptionItem *GetDescriptionItem(EGameModeType gameModeType)
	{
		for (int i = 0; i < DescriptionItems.Num(); i++)
		{
			if (DescriptionItems[i].GetGameModeType() == gameModeType)
				return &DescriptionItems[i];
		}
		return nullptr;
	}

protected:
	UPROPERTY()
	TArray<FGameTypeDescriptionItem> DescriptionItems;
};
