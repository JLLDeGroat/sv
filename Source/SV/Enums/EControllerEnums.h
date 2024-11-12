// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EControlLimit : uint8 {
	CL_NONE = 0,
	CL_NoClick = 1,
};

class SV_API EControllerEnums
{
public:
	template< typename T >
	static FORCEINLINE FString ToString(T EnumValue)
	{
		static_assert(TIsUEnumClass< T >::Value, "'T' template parameter to EnumToString must be a valid UEnum");
		return StaticEnum< T >()->GetNameStringByIndex((int32)EnumValue);
	}

	/*template<typename T>*/
	static FString GetControlLimitAsString(EControlLimit value)
	{
		UEnum* pEnum = FindObject<UEnum>(nullptr, L"/Script/SV.EControlLimit");
		return *(pEnum ? pEnum->GetNameStringByIndex(static_cast<uint8>(value)) : "null");
	}
};
