// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentInventoryHelpers.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "../../Utilities/SvUtilities.h"

UImage* UEquipmentInventoryHelpers::GetImageFromWidget(UUserWidget* widget, FString widgetName) {
	return (UImage*)widget->GetWidgetFromName(FName(widgetName));
}
UButton* UEquipmentInventoryHelpers::GetButtonFromWidget(UUserWidget* widget, FString widgetName) {
	return (UButton*)widget->GetWidgetFromName(FName(widgetName));
}
UTextBlock* UEquipmentInventoryHelpers::GetTextBlockFromWidget(UUserWidget* widget, FString widgetName) {
	return (UTextBlock*)widget->GetWidgetFromName(FName(widgetName));
}

UTexture2D* UEquipmentInventoryHelpers::GetTextureForGun(EGun gunType) {
	auto string = "/Script/Engine.Texture2D'/Game/Images/EmptyPrimarySlot.EmptyPrimarySlot'";

	switch (gunType) {
	case EGun::G_PeaRifle:
		string = "/Script/Engine.Texture2D'/Game/Images/PeaRifleImage.PeaRifleImage'";
		break;
	}

	return USvUtilities::GetTexture(string);
}

UTexture2D* UEquipmentInventoryHelpers::GetTextureForTool(EToolType toolType, uint8 tool) {
	auto string = "/Script/Engine.Texture2D'/Game/Images/EmptyToolSlot.EmptyToolSlot'";
	switch (toolType) {
	case EToolType::TT_Throwable:
	{
		switch ((EThrowable)tool) {
		case EThrowable::T_Grenade:
			string = "/Script/Engine.Texture2D'/Game/Images/GrenadeToolSlot.GrenadeToolSlot'";
			break;
		}
		break;
	}
	}
	return USvUtilities::GetTexture(string);
}

UTexture2D* UEquipmentInventoryHelpers::GetPrimaryNonTexture() {
	return USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/EmptyPrimarySlot.EmptyPrimarySlot'");
}
UTexture2D* UEquipmentInventoryHelpers::GetEquipmentNonTexture() {
	return USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/EmptyToolSlot.EmptyToolSlot'");
}