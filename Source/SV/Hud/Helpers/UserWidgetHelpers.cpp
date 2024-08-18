// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgetHelpers.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UImage* UUserWidgetHelpers::GetImageFromWidget(UUserWidget* widget, FString widgetName) {
	return (UImage*)widget->GetWidgetFromName(FName(widgetName));
}
UButton* UUserWidgetHelpers::GetButtonFromWidget(UUserWidget* widget, FString widgetName) {
	return (UButton*)widget->GetWidgetFromName(FName(widgetName));
}
UTextBlock* UUserWidgetHelpers::GetTextBlockFromWidget(UUserWidget* widget, FString widgetName) {
	return (UTextBlock*)widget->GetWidgetFromName(FName(widgetName));
}

UTexture2D* UUserWidgetHelpers::GetTextureForGun(EGun gunType) {
	auto string = "/Script/Engine.Texture2D'/Game/Images/EmptyPrimarySlot.EmptyPrimarySlot'";

	switch (gunType) {
	case EGun::G_PeaRifle:
		string = "/Script/Engine.Texture2D'/Game/Images/PeaRifleImage.PeaRifleImage'";
		break;
	}

	return USvUtilities::GetTexture(string);
}

UTexture2D* UUserWidgetHelpers::GetTextureForTool(EToolType toolType, uint8 tool) {
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

UTexture2D* UUserWidgetHelpers::GetPrimaryNonTexture() {
	return USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/EmptyPrimarySlot.EmptyPrimarySlot'");
}
UTexture2D* UUserWidgetHelpers::GetEquipmentNonTexture() {
	return USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/EmptyToolSlot.EmptyToolSlot'");
}

UTexture2D* UUserWidgetHelpers::GetTextureForActionType(EActionType actionType) {
	UTexture2D* texture = nullptr;
	switch (actionType) {
	case EActionType::AT_Extract:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/ExtractIcon.ExtractIcon'");
		break;
	case EActionType::AT_Grenade:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/GrenadeIcon.GrenadeIcon'");
		break;
	case EActionType::AT_Overwatch:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/OverwatchIcon.OverwatchIcon'");
		break;
	case EActionType::AT_Reload:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/ReloadIcon.ReloadIcon'");
		break;
	case EActionType::AT_Shoot:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/TargetIcon.TargetIcon'");
		break;
	case EActionType::AT_Sleep:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/EndTurnIcon.EndTurnIcon'");
		break;
	case EActionType::AT_Pickup:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/PickupIcon.PickupIcon'");
		break;
	}
	return texture;
}


void UUserWidgetHelpers::DesignButton(UUserWidget* widget, FString btnName) {
	auto btn = GetButtonFromWidget(widget, btnName);
	if (btn) {
		auto normalTexture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/BasicButton.BasicButton'");
		auto hoveredTexture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/BasicButtonHovered.BasicButtonHovered'");
		auto pressedTexture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/BasicButtonSelected.BasicButtonSelected'");


		auto style = FButtonStyle();
		style.Normal.SetResourceObject(normalTexture);
		style.Normal.OutlineSettings.CornerRadii = FVector4(10);
		style.Normal.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;

		style.Pressed.SetResourceObject(pressedTexture);
		style.Pressed.OutlineSettings.CornerRadii = FVector4(10);
		style.Pressed.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;

		style.Hovered.SetResourceObject(hoveredTexture);
		style.Hovered.OutlineSettings.CornerRadii = FVector4(10);
		style.Hovered.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;

		btn->SetStyle(style);
	}
	else
		UDebugMessages::LogError(widget, "failed to get button for design");
}