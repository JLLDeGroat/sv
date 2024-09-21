// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Enums/ECharacterEnums.h"
#include "HudDelegates.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHudItemHovered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHudItemUnhovered);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClearTargetDataHud);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAddTargetDataToHud, FGuid, Id, FVector, SourceLocation, FVector, TargetLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTargetIconClicked, FGuid, Id, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimTargetVisibility, bool, bVisibility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddActionIconToHud, EActionType, ActionType, FString, ShortCutLetter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetActionIcons);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddSoldierToCharacterTileWidget, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelectNextCharacterWithAp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckCharacterTileIsActive, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddSoldierToCharacterDetailsWidget, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRefreshCharacterDetailsWidget);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHideOrResetUIWidget);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetCharacterTileWidget);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCycleToNextTarget);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterTileVisibility, bool, bVisibility);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowMissionCompleteWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowMissionFailedWidget);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateMainObjectiveText, FString, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSubObjectiveOneText, FString, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSubObjectiveTwoText, FString, Text);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceChange, EResourceType, ResourceType);


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

	FOnHudItemHovered _OnHudItemHovered;
	FOnHudItemUnhovered _OnHudItemUnhovered;

	FAddTargetDataToHud _AddTargetDataToHud;
	FClearTargetDataHud _ClearTargetDataHud;
	FTargetIconClicked _TargetIconClicked;
	FAimTargetVisibility _AimTargetVisibility;
	FAddActionIconToHud _AddActionIconToHud;
	FResetActionIcons _ResetActionIcons;
	FAddSoldierToCharacterTileWidget _AddSoldierToCharacterTileWidget;
	FCheckCharacterTileIsActive _CheckCharacterTileIsActive;
	FAddSoldierToCharacterDetailsWidget _AddSoldierToCharacterDetailsWidget;
	FRefreshCharacterDetailsWidget _RefreshCharacterDetailsWidget;
	FHideOrResetUIWidget _HideOrResetUIWidget;
	FResetCharacterTileWidget _ResetCharacterTileWidget;
	FCycleToNextTarget _CycleToNextTarget;
	FSelectNextCharacterWithAp _SelectNextCharacterWithAp;

	FCharacterTileVisibility _CharacterTileVisibility;

	FShowMissionCompleteWidget _ShowMissionCompleteWidget;
	FShowMissionFailedWidget _ShowMissionFailedWidget;

	FUpdateMainObjectiveText _UpdateMainObjectiveText;
	FUpdateSubObjectiveOneText _UpdateSubObjectiveOneText;
	FUpdateSubObjectiveTwoText _UpdateSubObjectiveTwoText;

	FOnResourceChange _OnResourceChange;
};
