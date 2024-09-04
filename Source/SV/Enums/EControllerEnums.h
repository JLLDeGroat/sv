// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class SV_API EControllerEnums
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
enum class EControlLimit : uint8 {
	CL_NONE = 0,
	CL_NoClick = 1,
};