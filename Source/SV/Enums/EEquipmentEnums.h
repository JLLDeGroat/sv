// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */

UENUM(BlueprintType)
enum class EThrowable : uint8 {
	INVALID = 0,
	T_Grenade = 1,
};

UENUM(BlueprintType)
enum class EGun : uint8 {
	INVALID = 0,
	G_PeaRifle = 1,
	G_UrfGun = 2,
};


UENUM(BlueprintType)
enum class EToolType : uint8 {
	INVALID = 0,
	TT_Throwable = 1,
	TT_HealthKit = 2,
};


UENUM(BlueprintType)
enum class EAttachType : uint8 {
	INVALID = 0,
	AT_Backpack,
	AT_RightHand,
	AT_RightHolster,
};

UENUM(BlueprintType)
enum class EResourceType : uint8 {
	INVALID = 0,
	RT_Currency = 1,
};

class SV_API EEquipmentEnums
{
public:
	template< typename T >
	static FORCEINLINE FString ToString(T EnumValue)
	{
		static_assert(TIsUEnumClass< T >::Value, "'T' template parameter to EnumToString must be a valid UEnum");
		return StaticEnum< T >()->GetNameStringByIndex((int32)EnumValue);
	}

	static FORCEINLINE FString GetResourceTypeAsString(EResourceType rType) {
		UEnum* EnumPtr = FindObject<UEnum>(GetTransientPackage(), TEXT("EResourceType"), true);
		if (EnumPtr)
			return EnumPtr->GetNameStringByValue((int64)rType);
		else
			return "FOUND_INVALID";
	}
};
