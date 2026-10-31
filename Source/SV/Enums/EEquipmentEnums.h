// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */

UENUM(BlueprintType)
enum class EThrowable : uint8
{
	INVALID = 0,
	T_Grenade = 1,
};

UENUM(BlueprintType)
enum class EHealthKits : uint8
{
	INVALID = 0,
	HK_Basic = 1,
	HK_Large = 2,
	HK_Antidote = 3,
	HK_Antiburn
};

UENUM(BlueprintType)
enum class EGun : uint8
{
	INVALID = 0,
	G_PeaRifle = 1,
	G_UrfGun = 2,
	G_ConstructRifle = 3,
	G_ConstructLaser = 4,
};

UENUM(BlueprintType)
enum class EToolType : uint8
{
	INVALID = 0,
	TT_Throwable = 1,
	TT_HealthKit = 2,
};

UENUM(BlueprintType)
enum class EAttachType : uint8
{
	INVALID = 0,
	AT_Backpack,
	AT_RightHand,
	AT_RightArm,
	AT_RightHolster,

	AT_LeftHand,
	AT_LeftArm,

	AT_Helmet,
	AT_LeftShoulder,
	AT_RightShoulder,
	AT_LeftArmGuard,
	AT_RightArmGuard,
	AT_LeftWristGuard,
	AT_RightWristGuard,
	AT_FrontChestGuard,
	AT_BackChestGuard,
	AT_LeftThighGuard,
	AT_RightThighGuard,
	AT_LeftShinGuard,
	AT_RightShinGuard,
};

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	INVALID = 0,
	RT_Currency = 1,
};

UENUM(BlueprintType)
enum class EClothingType : uint8
{
	INVALID = 0,
	CT_Chest  =1,
	CT_Legs = 2,
	CT_Arms = 3,
	CT_Shoulders = 4,
	CT_Helmet = 5,
};

UENUM(BlueprintType)
enum class EClothingGrade : uint8
{
	INVALID = 0,
	CG_Iron = 1,
};

class SV_API EEquipmentEnums
{
public:
	template <typename T>
	static FORCEINLINE FString ToString(T EnumValue)
	{
		static_assert(TIsUEnumClass<T>::Value, "'T' template parameter to EnumToString must be a valid UEnum");
		return StaticEnum<T>()->GetNameStringByIndex((int32)EnumValue);
	}

	static FORCEINLINE FString GetResourceTypeAsString(EResourceType rType)
	{
		UEnum *EnumPtr = FindObject<UEnum>(GetTransientPackage(), TEXT("EResourceType"), EFindObjectFlags::ExactClass);
		if (EnumPtr)
			return EnumPtr->GetNameStringByValue((int64)rType);
		else
			return "FOUND_INVALID";
	}
};
