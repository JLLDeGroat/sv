// Fill out your copyright notice in the Description page of Project Settings.

#include "CrewDetailsGridWidget.h"
#include "../../../../../Utilities/SvUtilities.h"
#include "../../../../../Instance/SvGameInstance.h"
#include "../../../../../Instance/Managers/CurrentGameDataManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "../../../../Helpers/UserWidgetHelpers.h"
#include "SelectEquipmentListWidget.h"

void UCrewDetailsGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);

	if (PrimaryBtn)
	{
		PrimaryBtn->OnClicked.AddDynamic(this, &UCrewDetailsGridWidget::PrimaryClicked);
		UUserWidgetHelpers::DesignButtonTextOnly(PrimaryBtn);
	}
	auto equipmentButton1 = UUserWidgetHelpers::GetButtonFromWidget(EquipmentSlot1, "EquipmentButton");
	if (equipmentButton1)
	{
		equipmentButton1->OnClicked.AddDynamic(this, &UCrewDetailsGridWidget::Tool1Clicked);
		UUserWidgetHelpers::DesignButtonTextOnly(equipmentButton1);
		UUserWidgetHelpers::SetButtonText(equipmentButton1, "Tool 1");
	}
	auto equipmentButton2 = UUserWidgetHelpers::GetButtonFromWidget(EquipmentSlot2, "EquipmentButton");
	if (equipmentButton2)
	{
		equipmentButton2->OnClicked.AddDynamic(this, &UCrewDetailsGridWidget::Tool2Clicked);
		UUserWidgetHelpers::DesignButtonTextOnly(equipmentButton2);
		UUserWidgetHelpers::SetButtonText(equipmentButton2, "Tool 2");
	}

	auto equipmentButton3 = UUserWidgetHelpers::GetButtonFromWidget(EquipmentSlot3, "EquipmentButton");
	if (equipmentButton3)
	{
		equipmentButton3->OnClicked.AddDynamic(this, &UCrewDetailsGridWidget::Tool3Clicked);
		UUserWidgetHelpers::DesignButtonTextOnly(equipmentButton3);
		UUserWidgetHelpers::SetButtonText(equipmentButton3, "Tool 3");
	}

	CurrentCrewId = FGuid::FGuid();
}
#pragma optimize("", off)
void UCrewDetailsGridWidget::InitialiseGridForCrewMember(FGuid crewMember)
{
	auto instance = USvUtilities::GetGameInstance(GetWorld());

	if (!instance || !instance->GetCurrentGameDataManager())
		return UDebugMessages::LogError(this, "failed to get instance/gamedatamanager");

	ResetDetailsGrid();

	auto gameDataManager = instance->GetCurrentGameDataManager();
	auto gameData = gameDataManager->GetCurrentGameData();
	auto crewMemberData = gameData->GetCrewMember(crewMember);
	auto primary = gameData->GetCrewPrimary(crewMemberData->GetId());

	CurrentCrewId = crewMemberData->GetId();

	if (PrimaryImage)
	{
		auto requiredGunType = EGun::INVALID;
		if (primary)
			requiredGunType = primary->GetPrimaryGunType();
		PrimaryImage->SetBrushFromTexture(UUserWidgetHelpers::GetTextureForGun(requiredGunType));
	}
	else
		return UDebugMessages::LogError(this, "failed to get primary image btn");

	auto crewMemberTools = gameData->GetToolsFromMember(CurrentCrewId);

	for (int i = 0; i < crewMemberTools.Num(); i++)
	{
		auto toolSlot = i + 1;
		auto equipmentSlotWidget = (UUserWidget *)GetWidgetFromName(FName("EquipmentSlot" + FString::SanitizeFloat(toolSlot, 0)));

		auto equipmentImage = UUserWidgetHelpers::GetImageFromWidget(equipmentSlotWidget, "EquipmentImage");
		if (equipmentImage)
		{
			equipmentImage->SetBrushFromTexture(UUserWidgetHelpers::GetTextureForTool(crewMemberTools[i]->GetToolType(), crewMemberTools[i]->GetTool()));
		}
		else
			return UDebugMessages::LogError(this, "failed to get equpment image" + FString::SanitizeFloat(toolSlot, 0));
	}
}
#pragma optimize("", on)

void UCrewDetailsGridWidget::PrimaryClicked()
{
	UDebugMessages::LogDisplay(this, "Primary Clicked");
	auto owningWidget = (UUserWidget *)GetOuter()->GetOuter();
	if (owningWidget)
	{
		auto equipmentSelectWidget = (USelectEquipmentListWidget *)owningWidget->GetWidgetFromName("SelectPrimaryList");
		if (equipmentSelectWidget)
		{
			equipmentSelectWidget->OpenListForPrimaries(CurrentCrewId);
		}
	}
}

void UCrewDetailsGridWidget::Tool1Clicked()
{
	OpenToolSelectionMenu(0);
}
void UCrewDetailsGridWidget::Tool2Clicked()
{
	OpenToolSelectionMenu(1);
}
void UCrewDetailsGridWidget::Tool3Clicked()
{
	OpenToolSelectionMenu(2);
}

void UCrewDetailsGridWidget::OpenToolSelectionMenu(int toolIndex)
{
	auto owningWidget = (UUserWidget *)GetOuter()->GetOuter();
	if (owningWidget)
	{
		auto equipmentSelectWidget = (USelectEquipmentListWidget *)owningWidget->GetWidgetFromName("SelectPrimaryList");
		if (equipmentSelectWidget)
		{
			equipmentSelectWidget->OpenListForTools(CurrentCrewId, toolIndex);
		}
	}
}

void UCrewDetailsGridWidget::ResetDetailsGrid()
{
	auto eq1ImageSlot = UUserWidgetHelpers::GetImageFromWidget(EquipmentSlot1, "EquipmentImage");
	auto eq2ImageSlot = UUserWidgetHelpers::GetImageFromWidget(EquipmentSlot2, "EquipmentImage");
	auto eq3ImageSlot = UUserWidgetHelpers::GetImageFromWidget(EquipmentSlot3, "EquipmentImage");

	if (PrimaryImage)
		ResetPrimaryInventoryImage(PrimaryImage);

	if (eq1ImageSlot)
		ResetEquipmentInventoryImage(eq1ImageSlot);
	if (eq2ImageSlot)
		ResetEquipmentInventoryImage(eq2ImageSlot);
	if (eq3ImageSlot)
		ResetEquipmentInventoryImage(eq3ImageSlot);
}

void UCrewDetailsGridWidget::ResetPrimaryInventoryImage(UImage *image)
{
	image->SetBrushFromTexture(UUserWidgetHelpers::GetPrimaryNonTexture());
}
void UCrewDetailsGridWidget::ResetEquipmentInventoryImage(UImage *image)
{
	image->SetBrushFromTexture(UUserWidgetHelpers::GetEquipmentNonTexture());
}