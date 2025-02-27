// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class SV_API EEnvironmentEnums
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
enum class EIndicatorType : uint8 {
	IT_NONE = 0,
	IT_Interact = 1,
	IT_Extract = 2,
	IT_Spawn = 3
};

UENUM(BlueprintType)
enum class EConstructionType : uint8 {
	IT_NONE = 0,
	IT_RollingTable = 1,
	IT_LongTable = 2,
	IT_CornerTable = 3
};
