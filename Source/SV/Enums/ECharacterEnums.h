// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class SV_API ECharacterEnums
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
enum class ECharacterControl : uint8 {
	INVALID = 0,
	CC_AI = 1,
	CC_Player = 2
};


UENUM(BlueprintType)
enum class EAttackType : uint8 {
	INVALID = 0,
	AT_NONE = 1,
	AT_BasicFire = 2,
	AT_MoveAndFire_Left = 3,
	AT_MoveAndFire_Right = 4,
	AT_BasicMelee = 5,
};

UENUM(BlueprintType)
enum class ECameraState : uint8 {
	INVALID = 0,
	CS_Control = 1,
	CS_GunTarget = 2,
	CS_GunShoot = 3,
};


UENUM(BlueprintType)
enum class EAttackState : uint8 {
	INVALID = 0,
	CS_NONE = 1,
	CS_RotatingToMelee = 2,
	CS_Attacking = 3,
	CS_MoveAttack = 4,
	CS_Return = 5,
	CS_RotatingToShoot = 6,
};