// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgetHelpers.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
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
	case EActionType::AT_Interact:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/Interact.Interact'");
		break;
	case EActionType::AT_SwapWeapon:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/WeaponSwap.WeaponSwap'");
		break;
	case EActionType::AT_BasicHealthKit:
	case EActionType::AT_LargeHealthKit:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/HealIcon.HealIcon'");
		break;
	}
	return texture;
}

UTexture2D* UUserWidgetHelpers::GetTextureForResourceType(EResourceType rType) {
	UTexture2D* texture = nullptr;
	switch (rType) {
	case EResourceType::RT_Currency:
		texture = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/ScrapIcon.ScrapIcon'");
		break;
	}
	return texture;
}

UTexture2D* UUserWidgetHelpers::GetRandomMissionImage() {
	auto random = FMath::RandRange(1, 12);
	auto reference = "/Script/Engine.Texture2D'/Game/Images/Levels/levelImage" + FString::SanitizeFloat(random, 0) + ".levelImage" + FString::SanitizeFloat(random, 0) + "'";

	auto texture = USvUtilities::GetTexture(reference);
	return texture;
}

void UUserWidgetHelpers::DesignButton(UUserWidget* widget, FString btnName, int fontSize) {
	auto btn = GetButtonFromWidget(widget, btnName);
	if (btn)
		DesignButton(btn, fontSize);
	else
		UDebugMessages::LogError(widget, "failed to get button for design");
}

void UUserWidgetHelpers::DesignButton(UButton* btn, int fontSize) {
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

	UWidget* ButtonContent = btn->GetChildAt(0);
	if (UTextBlock* TextBlock = Cast<UTextBlock>(ButtonContent))
		UUserWidgetHelpers::DesignText(TextBlock, fontSize);

	btn->SetStyle(style);
}

void UUserWidgetHelpers::DesignButtonTextOnly(UButton* btn, int fontSize) {
	UWidget* ButtonContent = btn->GetChildAt(0);
	if (UTextBlock* TextBlock = Cast<UTextBlock>(ButtonContent))
		UUserWidgetHelpers::DesignText(TextBlock, fontSize);
}

void UUserWidgetHelpers::SetButtonText(UButton* btn, FString text) {
	UWidget* ButtonContent = btn->GetChildAt(0);
	if (UTextBlock* TextBlock = Cast<UTextBlock>(ButtonContent)) {
		TextBlock->SetText(FText::FromString(text));
	}
}

void UUserWidgetHelpers::DesignText(UTextBlock* textBlock, int fontSize) {
	// /Script/Engine.Font'/Game/Fonts/Aaargh_Font.Aaargh_Font'
	// /Script/Engine.Font'/Game/Fonts/AARDC____Font.AARDC____Font'
	// /Script/Engine.Font'/Game/Fonts/Urba_Font.Urba_Font'
	FString reference = "/Script/Engine.Font'/Game/Fonts/Urba_Font.Urba_Font'";
	auto fontObj = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));

	if (!fontObj)
		UDebugMessages::LogError(textBlock, "failed to get fontObj");

	FSlateFontInfo FontInfo;
	FontInfo.FontObject = fontObj;
	FontInfo.Size = fontSize;
	textBlock->SetFont(FontInfo);
}