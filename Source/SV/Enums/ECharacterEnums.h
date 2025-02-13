// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
UENUM(BlueprintType)
enum class ECharacterControl : uint8 {
	INVALID = 0,
	CC_AI = 1,
	CC_Player = 2
};

UENUM(BlueprintType)
enum class ECharacterAnimState : uint8 {
	INVALID = 0,
	AS_AR = 1,
	AS_PISTOL = 2
};

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	INVALID = 0,
	AT_NONE = 1,
	AT_BasicFire = 2,
	AT_MoveAndFire_Left = 3,
	AT_MoveAndFire_Right = 4,
	AT_BasicMelee = 5,
	AT_BasicThrow = 6,
};

UENUM(BlueprintType)
enum class ECameraState : uint8 {
	INVALID = 0,
	CS_Control,
	CS_GunTarget,
	CS_GunShoot,
	CS_ReTarget,
	CS_CinematicShoot,
	CS_OverwatchCinematicShoot,
	CS_ThrowTarget,
	CS_Throw,
	CS_Overwatch,
	CS_ToolUsage,
	CS_CharacterDeath
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

UENUM(BlueprintType)
enum class EActionType :uint8 {
	INVALiD = 0,
	AT_Shoot = 1,
	AT_Grenade = 2,
	AT_Sleep = 3,
	AT_Reload = 4,
	AT_Overwatch = 5,
	AT_Extract = 6,
	AT_Pickup = 7,
	AT_Interact = 8,
	AT_SwapWeapon = 9,
	AT_BasicHealthKit = 10,
	AT_LargeHealthKit = 11,
	AT_AnditodeKit = 12,
	AT_FlameRetardentKit = 13,

};

UENUM(BlueprintType)
enum class ETargetIcon :uint8 {
	INVALiD = 0,
	TI_ConstructRange = 1,
	TI_ConstructMelee = 2
};

UENUM(BlueprintType)
enum class EBloodType :uint8 {
	INVALiD = 0,
	BT_RedBlood = 1,
	BT_PurpleCrystal = 2
};

UENUM(BlueprintType)
enum class EDebuffType : uint8 {
	INVALID = 0,
	DBT_Accuracy = 1,
};

class SV_API ECharacterEnums
{
public:
	template< typename T >
	static FORCEINLINE FString ToString(T EnumValue)
	{
		static_assert(TIsUEnumClass< T >::Value, "'T' template parameter to EnumToString must be a valid UEnum");
		return StaticEnum< T >()->GetNameStringByIndex((int32)EnumValue);
	}

	static FORCEINLINE FString GetCameraStateAsString(ECameraState cState) {
		UEnum* EnumPtr = FindObject<UEnum>(GetTransientPackage(), TEXT("ECameraState"), true);
		if (EnumPtr)
			return EnumPtr->GetNameStringByValue((int64)cState);
		else
			return "FOUND_INVALID";
	}
};