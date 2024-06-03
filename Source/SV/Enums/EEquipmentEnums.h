// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class SV_API EEquipmentEnums
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
enum class EThrowable : uint8 {
	INVALID = 0,
	T_Grenade = 1,
};

UENUM(BlueprintType)
enum class EAttachType : uint8 {
	INVALID = 0,
	AT_Backpack,
	AT_RightHand
};


