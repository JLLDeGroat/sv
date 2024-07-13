// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewDetailsGridWidget.h"
#include "../../../../../Utilities/SvUtilities.h"
#include "../../../../../Instance/SvGameInstance.h"
#include "../../../../../Instance/Managers/CurrentGameDataManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "../../../../Helpers/EquipmentInventoryHelpers.h"
#include "SelectEquipmentListWidget.h"

void UCrewDetailsGridWidget::NativeConstruct() {
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);

	auto primaryButton = UEquipmentInventoryHelpers::GetButtonFromWidget(this, "PrimaryBtn");
	if (primaryButton)
		primaryButton->OnClicked.AddDynamic(this, &UCrewDetailsGridWidget::PrimaryClicked);

	auto equipmentImage1 = (UUserWidget*)GetWidgetFromName(FName("EquipmentSlot1"));
	auto equipmentButton1 = UEquipmentInventoryHelpers::GetButtonFromWidget(equipmentImage1, "EquipmentButton");
	if (equipmentButton1)
		equipmentButton1->OnClicked.AddDynamic(this, &UCrewDetailsGridWidget::Tool1Clicked);

	auto equipmentImage2 = (UUserWidget*)GetWidgetFromName(FName("EquipmentSlot2"));
	auto equipmentButton2 = UEquipmentInventoryHelpers::GetButtonFromWidget(equipmentImage2, "EquipmentButton");
	if (equipmentButton2)
		equipmentButton2->OnClicked.AddDynamic(this, &UCrewDetailsGridWidget::Tool2Clicked);

	auto equipmentImage3 = (UUserWidget*)GetWidgetFromName(FName("EquipmentSlot3"));
	auto equipmentButton3 = UEquipmentInventoryHelpers::GetButtonFromWidget(equipmentImage3, "EquipmentButton");
	if (equipmentButton3)
		equipmentButton3->OnClicked.AddDynamic(this, &UCrewDetailsGridWidget::Tool3Clicked);


	CurrentCrewId = FGuid::FGuid();
}

void UCrewDetailsGridWidget::InitialiseGridForCrewMember(FGuid crewMember) {
	auto instance = USvUtilities::GetGameInstance(GetWorld());

	if (!instance || !instance->GetCurrentGameDataManager())
		return UDebugMessages::LogError(this, "failed to get instance/gamedatamanager");

	ResetDetailsGrid();

	auto gameDataManager = instance->GetCurrentGameDataManager();
	auto gameData = gameDataManager->GetCurrentGameData();
	auto crewMemberData = gameData->GetCrewMember(crewMember);
	auto primary = gameData->GetCrewPrimary(crewMemberData.GetId());

	CurrentCrewId = crewMemberData.GetId();

	auto primaryImageSlot = UEquipmentInventoryHelpers::GetImageFromWidget(this, "PrimaryImage");
	if (primaryImageSlot) {
		primaryImageSlot->SetBrushFromTexture(UEquipmentInventoryHelpers::GetTextureForGun(primary.GetPrimaryGunType()));
	}
	else return UDebugMessages::LogError(this, "failed to get primary image btn");

	auto crewMemberTools = gameData->GetToolsFromMember(CurrentCrewId);

	for (int i = 0; i < crewMemberTools.Num(); i++) {
		auto toolSlot = i + 1;
		auto equipmentSlotWidget = (UUserWidget*)GetWidgetFromName(FName("EquipmentSlot" + FString::SanitizeFloat(toolSlot, 0)));

		auto equipmentImage = UEquipmentInventoryHelpers::GetImageFromWidget(equipmentSlotWidget, "EquipmentImage");
		if (equipmentImage) {
			equipmentImage->SetBrushFromTexture(UEquipmentInventoryHelpers::GetTextureForTool(crewMemberTools[i]->GetToolType(), crewMemberTools[i]->GetTool()));
		}
		else return UDebugMessages::LogError(this, "failed to get equpment image" + FString::SanitizeFloat(toolSlot, 0));
	}
}


void UCrewDetailsGridWidget::PrimaryClicked() {
	UDebugMessages::LogDisplay(this, "Primary Clicked");

	auto owningWidget = (UUserWidget*)GetOuter()->GetOuter();
	if (owningWidget) {
		auto equipmentSelectWidget = (USelectEquipmentListWidget*)owningWidget->GetWidgetFromName("SelectPrimaryList");
		if (equipmentSelectWidget) {
			equipmentSelectWidget->OpenListForPrimaries(CurrentCrewId);
		}
	}
}

void UCrewDetailsGridWidget::Tool1Clicked() {
	OpenToolSelectionMenu(0);
}
void UCrewDetailsGridWidget::Tool2Clicked() {
	OpenToolSelectionMenu(1);
}
void UCrewDetailsGridWidget::Tool3Clicked() {
	OpenToolSelectionMenu(2);
}

void UCrewDetailsGridWidget::OpenToolSelectionMenu(int toolIndex) {
	auto owningWidget = (UUserWidget*)GetOuter()->GetOuter();
	if (owningWidget) {
		auto equipmentSelectWidget = (USelectEquipmentListWidget*)owningWidget->GetWidgetFromName("SelectPrimaryList");
		if (equipmentSelectWidget) {
			equipmentSelectWidget->OpenListForTools(CurrentCrewId, toolIndex);
		}
	}
}

void UCrewDetailsGridWidget::ResetDetailsGrid() {
	auto primaryImageSlot = UEquipmentInventoryHelpers::GetImageFromWidget(this, "PrimaryImage");

	auto equipmentImage1 = (UUserWidget*)GetWidgetFromName(FName("EquipmentSlot1"));
	auto equipmentImage2 = (UUserWidget*)GetWidgetFromName(FName("EquipmentSlot2"));
	auto equipmentImage3 = (UUserWidget*)GetWidgetFromName(FName("EquipmentSlot3"));
	auto eq1ImageSlot = UEquipmentInventoryHelpers::GetImageFromWidget(equipmentImage1, "EquipmentImage");
	auto eq2ImageSlot = UEquipmentInventoryHelpers::GetImageFromWidget(equipmentImage2, "EquipmentImage");
	auto eq3ImageSlot = UEquipmentInventoryHelpers::GetImageFromWidget(equipmentImage3, "EquipmentImage");

	if (primaryImageSlot) ResetPrimaryInventoryImage(primaryImageSlot);

	if (eq1ImageSlot) ResetEquipmentInventoryImage(eq1ImageSlot);
	if (eq2ImageSlot) ResetEquipmentInventoryImage(eq2ImageSlot);
	if (eq3ImageSlot) ResetEquipmentInventoryImage(eq3ImageSlot);
}


void UCrewDetailsGridWidget::ResetPrimaryInventoryImage(UImage* image) {
	image->SetBrushFromTexture(UEquipmentInventoryHelpers::GetPrimaryNonTexture());
}
void UCrewDetailsGridWidget::ResetEquipmentInventoryImage(UImage* image) {
	image->SetBrushFromTexture(UEquipmentInventoryHelpers::GetEquipmentNonTexture());
}