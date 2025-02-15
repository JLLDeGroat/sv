// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class SV_API EHudEnums
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
enum class EWorldPage : uint8
{
	INVALID = 0,
	WP_World = 1,
	WP_Squad = 2,
	WP_Supplies = 3,
};