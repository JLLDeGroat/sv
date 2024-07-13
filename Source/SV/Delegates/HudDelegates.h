// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HudDelegates.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClearTargetDataHud);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAddTargetDataToHud, FGuid, Id, FVector, SourceLocation, FVector, TargetLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTargetIconClicked, FGuid, Id, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimTargetVisibility, bool, bVisibility);

/**
 * 
 */
UCLASS()
class SV_API UHudDelegates : public UObject
{
	GENERATED_BODY()
public:
	static UHudDelegates* GetInstance() {
		static UHudDelegates* hudDelegates = 0; if (!hudDelegates)
		{
			hudDelegates = NewObject<UHudDelegates>();
			hudDelegates->AddToRoot();
		}
		return hudDelegates;
	}

	FAddTargetDataToHud _AddTargetDataToHud;
	FClearTargetDataHud _ClearTargetDataHud;
	FTargetIconClicked _TargetIconClicked;
	FAimTargetVisibility _AimTargetVisibility;
};
