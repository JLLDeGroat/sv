// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */

class SV_API EAIEnums
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
enum class EAIBehaviourMoveRoutes : uint8 {
	INVALID = 0,
	BMR_Melee,
	BMR_Range
};

UENUM(BlueprintType)
enum class EAIBehaviourAttackRoutes : uint8 {
	INVALID = 0,
	BAR_Melee,
	BAR_Range,
	BAR_Suicide
};