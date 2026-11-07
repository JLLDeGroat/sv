// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthAndStatusWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Helpers/UserWidgetHelpers.h"

DEFINE_LOG_CATEGORY(SVLogWidgetAnimation);

void UHealthAndStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayAnimationForward(AnimateDamageOut);
	PlayAnimationForward(AnimateStatusOut);

	UUserWidgetHelpers::DesignText(DamageText, 10, FLinearColor(208, 0.2f, 0.5f, 1));
	UUserWidgetHelpers::DesignText(NameTag, 9);
}

UProgressBar *UHealthAndStatusWidget::GetHealthBar()
{
	return HealthBar;
}
UProgressBar *UHealthAndStatusWidget::GetDynamicHealthBar()
{
	return DynamicHealthBar;
}
UTextBlock *UHealthAndStatusWidget::GetNameTag()
{
	return NameTag;
}
UTextBlock *UHealthAndStatusWidget::GetDamageText()
{
	return DamageText;
}
UTextBlock *UHealthAndStatusWidget::GetStatusText()
{
	return StatusText;
}
UProgressBar *UHealthAndStatusWidget::GetArmourBarHelmet()
{
	return ArmourBarHelmet;
}
UProgressBar *UHealthAndStatusWidget::GetArmourBarHelmetDynamic()
{
	return ArmourBarHelmetDynamic;
}

UProgressBar *UHealthAndStatusWidget::GetArmourBarShoulder()
{
	return ArmourBarShoulder;
}
UProgressBar *UHealthAndStatusWidget::GetArmourBarShoulderDynamic()
{
	return ArmourBarShoulderDynamic;
}

UProgressBar *UHealthAndStatusWidget::GetArmourBarArms()
{
	return ArmourBarArms;
}
UProgressBar *UHealthAndStatusWidget::GetArmourBarArmsDynamic()
{
	return ArmourBarArmsDynamic;
}

UProgressBar *UHealthAndStatusWidget::GetArmourBarChest()
{
	return ArmourBarChest;
}
UProgressBar *UHealthAndStatusWidget::GetArmourBarChestDynamic()
{
	return ArmourBarChestDynamic;
}

UProgressBar *UHealthAndStatusWidget::GetArmourBarLegs()
{
	return ArmourBarLegs;
}
UProgressBar *UHealthAndStatusWidget::GetArmourBarLegsDynamic()
{
	return ArmourBarLegsDynamic;
}