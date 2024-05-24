// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enums/ECharacterEnums.h"
#include "SkillData.generated.h"
/**
 *
 */

USTRUCT()
struct FSkillDataItem
{
	GENERATED_BODY()
public:
	FSkillDataItem() {
		Name = "";
		Category = "";
		DamageType = "";
		DamageMax = 0;
		DamageMin = 0;
		ApCost = 0;
		AttackType = EAttackType::AT_NONE;
	}

	FString GetName() { return Name; }
	FString GetCategory() { return Category; }
	FString GetDamageType() { return DamageType; }

	int GetDamageMin() const { return DamageMin; }
	int GetDamageMax() const { return DamageMax; }

	int GetApCost() const { return ApCost; }

	EAttackType GetAttackType() const { return AttackType; }

protected:

	UPROPERTY() FString Name;
	UPROPERTY() FString Category;
	UPROPERTY() FString DamageType;

	UPROPERTY() int DamageMin;
	UPROPERTY() int DamageMax;

	UPROPERTY() int ApCost;

	UPROPERTY() EAttackType AttackType;
};


USTRUCT()
struct FSkillData
{
	GENERATED_BODY()
public:
	FSkillData() {

	}

	FORCEINLINE static FString MeleeDamageType() { return "melee"; }


	FORCEINLINE static FString ActivationCategory() { return "activation"; }


	FSkillDataItem* GetItemByName(FString name) { for (int i = 0; i < SkillDataItems.Num(); i++) if (SkillDataItems[i].GetName() == name) return &SkillDataItems[i]; return nullptr; }
	TArray<FSkillDataItem*> GetSkillDataItems() { TArray<FSkillDataItem*> arr; for (int i = 0; i < SkillDataItems.Num(); i++) arr.Emplace(&SkillDataItems[i]); return arr; }
protected:

	UPROPERTY() TArray<FSkillDataItem> SkillDataItems;
};
