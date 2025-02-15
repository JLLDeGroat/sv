// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

/**
 *
 */
class SV_API EConfigurationEnums{
	public :
		template <typename T>
		static FORCEINLINE FString ToString(T EnumValue){
			static_assert(TIsUEnumClass<T>::Value, "'T' template parameter to EnumToString must be a valid UEnum");
return StaticEnum<T>()->GetNameStringByIndex((int32)EnumValue);
}
}
;

UENUM(BlueprintType)
enum class EGameModeType : uint8
{
	INVALID = 0,
	EMT_Classic = 1,
};

UENUM(BlueprintType)
enum class EStockCategory : uint8
{
	INVALID = 0,
	SC_Primary,
	SC_Secondary,
	SC_Throwable,
	SC_Armour
};

UENUM(BlueprintType)
enum class ETutorials : uint8
{
	INVALID = 0,
	T_WorldNavigation = 1,
	T_EquipmentSelect = 2,
	T_Supplies = 3
};

UENUM(BlueprintType)
enum class EOverwatchResult : uint8
{
	INVALID = 0,
	OR_Started = 1,
	OR_Delay = 2,
	OR_Error = 3
};

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	INVALID = 0,
	MT_Horizontal,
	MT_Vertical_UP,
	MT_Vertical_DOWN,
};

UENUM(BlueprintType)
enum class EClimbType : uint8
{
	INVALID = 0,
	CT_Up = 1,
	CT_Down = 2,
};

UENUM(BlueprintType)
enum class EDDType : uint8
{
	INVALID = 0,
	DD_Deviation = 1,
	DD_Directive = 2
};