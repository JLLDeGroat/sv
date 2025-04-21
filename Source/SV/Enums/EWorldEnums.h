// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class SV_API EWorldEnums
{
public:
	template <typename T>
	static FORCEINLINE FString ToString(T EnumValue)
	{
		static_assert(TIsUEnumClass<T>::Value, "'T' template parameter to EnumToString must be a valid UEnum");
		return StaticEnum<T>()->GetNameStringByIndex((int32)EnumValue);
	}
};

UENUM(BlueprintType)
enum class EGridStoryType : uint8
{
	INVALID = 0,
	GST_Decision = 1,
	GST_Battle = 2,
};

UENUM(BlueprintType)
enum class EMissionType : uint8
{
	INVALID = 0,
	MT_Survive = 1,
	MT_Extraction = 2,
	MT_Traverse = 3,
	MT_Exterminate = 4
};

UENUM(BlueprintType)
enum class ELevelGenType : uint8
{
	Generic,
	TwoBuilding
};

UENUM(BlueprintType)
enum class EStatisticType : uint8
{
	INVALID = 0,
	ST_DAMAGE,
	ST_Kill,
	ST_MOVEMENT,

};

UENUM(BlueprintType)
enum class EDirectorStatType : uint8
{
	INVALID = 0,
	DS_NOISE = 1,
	DS_KILLS = 2,
	DS_DEATHS = 3,
	DS_MOVEMENT = 4
};
