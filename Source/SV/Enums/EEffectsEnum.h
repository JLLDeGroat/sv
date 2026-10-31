// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EBloodEffect : uint8
{
    INVALID = 0,
    BE_Blood = 1,
    BE_IronSpark = 2,
    BE_ConstructSpark = 3
};

class SV_API EEffectsEnums
{
public:
    template <typename T>
    static FORCEINLINE FString ToString(T EnumValue)
    {
        static_assert(TIsUEnumClass<T>::Value, "'T' template parameter to EnumToString must be a valid UEnum");
        return StaticEnum<T>()->GetNameStringByIndex((int32)EnumValue);
    }
};