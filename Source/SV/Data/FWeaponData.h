// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enums/EEquipmentEnums.h"
#include "FWeaponData.generated.h"

/**
 *
 */
USTRUCT()
struct FWeaponItem
{
	GENERATED_BODY()
public:
	FWeaponItem() {
		Name = "Untitled";
		Penetration = 1;
	}

	FString GetName() { return Name; }
	FString GetDescription() { return Description; }

	EThrowable GetThrowableType() { return ThrowableType; }
	EGun GetGunType() { return GunType; }
	EToolType GetToolType() { return ToolType; }

	int GetPenetration() { return Penetration; }

protected:
	UPROPERTY() FString Name;
	UPROPERTY() FString Description;
	UPROPERTY() EThrowable ThrowableType;
	UPROPERTY() EGun GunType;
	UPROPERTY() EToolType ToolType;
	UPROPERTY() int Penetration;
};


USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()
public:
	FWeaponData() {

	}

	FWeaponItem* GetGunWeapon(EGun gunType) {
		if (gunType == EGun::INVALID) return nullptr;

		for (int i = 0; i < WeaponItems.Num(); i++)
			if (WeaponItems[i].GetGunType() == gunType)
				return &WeaponItems[i];

		return nullptr;
	}

protected:

	UPROPERTY() TArray<FWeaponItem> WeaponItems;
};
