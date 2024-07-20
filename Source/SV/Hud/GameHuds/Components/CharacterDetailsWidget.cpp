// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDetailsWidget.h"
#include "../../../Delegates/HudDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Characters/Components/EquipmentComponent.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Helpers/EquipmentInventoryHelpers.h"
#include "../../../Equipment/Equipment.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCharacterDetailsWidget::NativeConstruct() {
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hudDelegates");

	hudDelegates->_AddSoldierToCharacterDetailsWidget.AddDynamic(this, &UCharacterDetailsWidget::ReceiveActor);
	hudDelegates->_RefreshCharacterDetailsWidget.AddDynamic(this, &UCharacterDetailsWidget::RefreshReceivedActor);

	hudDelegates->_HideOrResetUIWidget.AddDynamic(this, &UCharacterDetailsWidget::HideOrReset);
}

void UCharacterDetailsWidget::HideOrReset() {
	SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterDetailsWidget::ReceiveActor(AActor* actor) {
	SetVisibility(ESlateVisibility::Visible);
	CurrentActor = actor;
	UpdateUIWithActor();
}

void UCharacterDetailsWidget::RefreshReceivedActor() {
	if (CurrentActor) UpdateUIWithActor();
}

void UCharacterDetailsWidget::UpdateUIWithActor() {
	auto detailsComponent = CurrentActor->GetComponentByClass<UCharacterDetailsComponent>();
	if (!detailsComponent)
		return UDebugMessages::LogError(this, "failed to get detailsComponent");

	auto equipmentComp = CurrentActor->GetComponentByClass<UEquipmentComponent>();
	if (!equipmentComp)
		return UDebugMessages::LogError(this, "failed to get equipmentComponent");

	auto healthWidget = GetHealthProgress();
	if (healthWidget) healthWidget->SetPercent(detailsComponent->GetHealthAsPercentage());

	auto APMPText = "AP: " + FString::SanitizeFloat(detailsComponent->GetActionPoints(), 0) + "/" + FString::SanitizeFloat(detailsComponent->GetMaxActionPoints(), 0)
		+ "   "
		+ "MP: " + FString::SanitizeFloat(detailsComponent->GetMovementPoints(), 0) + "/" + FString::SanitizeFloat(detailsComponent->GetMaxMovementPoints(), 0);

	auto apmpWidget = GetAPMPTextBlock();
	if (apmpWidget) apmpWidget->SetText(FText::FromString(APMPText));

	auto equipment = equipmentComp->GetPrimaryEquipment();
	if (equipment) {
		auto equipmentDetailsComp = equipment->GetComponentByClass<UEquipmentDetailsComponent>();
		if (!equipmentDetailsComp)
			return UDebugMessages::LogError(this, "failed to get equipmentdetails component");

		if (equipmentDetailsComp->GetIsGun()) {
			auto ammo = (float)equipmentDetailsComp->GetRounds() / (float)equipmentDetailsComp->GetMaxRounds();
			auto image = UEquipmentInventoryHelpers::GetTextureForGun(equipmentDetailsComp->GetGunType());

			UDebugMessages::LogDisplay(this, "setting ammo as " + FString::SanitizeFloat(ammo));

			auto ammoProgress = GetAmmoProgress();
			if (ammoProgress) ammoProgress->SetPercent(ammo);

			auto imageWidget = GetCurrentWeaponImage();
			if (imageWidget) imageWidget->SetBrushFromTexture(image);
		}
		else {
			return UDebugMessages::LogError(this, "TODO equipment not accounted for on details widget");
		}
	}
}

UImage* UCharacterDetailsWidget::GetCurrentWeaponImage() {
	return (UImage*)GetWidgetFromName("WeaponImage");
}
UProgressBar* UCharacterDetailsWidget::GetAmmoProgress() {
	return (UProgressBar*)GetWidgetFromName("PrimaryAmmo");
}
UProgressBar* UCharacterDetailsWidget::GetHealthProgress() {
	return (UProgressBar*)GetWidgetFromName("Health");
}
UTextBlock* UCharacterDetailsWidget::GetAPMPTextBlock() {
	return (UTextBlock*)GetWidgetFromName("APMPText");
}