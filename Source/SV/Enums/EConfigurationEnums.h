// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

/**
 *
 */
class SV_API EConfigurationEnums
{
public:
	template< typename T >
	static FORCEINLINE FString ToString(T EnumValue)
	{
		static_assert(TIsUEnumClass< T >::Value, "'T' template parameter to EnumToString must be a valid UEnum");
		return StaticEnum< T >()->GetNameStringByIndex((int32)EnumValue);
	}
};


UENUM(BlueprintType)
enum class EGameModeType : uint8 {
	INVALID = 0,
	EMT_Classic = 1,
};




UENUM(BlueprintType)
enum class EStockCategory : uint8 {
	INVALID = 0,
	SC_Primary,
	SC_Secondary,
	SC_Throwable,
	SC_Armour
};

UENUM(BlueprintType)
enum class ETutorials : uint8 {
	INVALID = 0,
	T_WorldNavigation = 1,
	T_EquipmentSelect = 2,
	T_Supplies = 3
};

