// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthAndStatusWidgetComponent.h"
#include "EquipmentComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Utilities/GridUtilities.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/GridSlot.h"
#include "Animation/WidgetAnimation.h"
#include "../../Hud/CharacterWidgets/HealthAndStatusWidget.h"
#include "../../Equipment/Clothing/Clothing.h"
#include "../../Equipment/Clothing/Components/ClothingStatsComponent.h"

DEFINE_LOG_CATEGORY(SVLogHealthAndStatusWidgetComponent);

UHealthAndStatusWidgetComponent::UHealthAndStatusWidgetComponent()
{
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Characters/Widgets/HealthAndStatusWidgetComponent_Bp.HealthAndStatusWidgetComponent_Bp_C'"));
	if (UClass *hudUIWidgetClass = hudUIRef.TryLoadClass<UUserWidget>())
	{
		WidgetClass = hudUIWidgetClass;
	}
	SetDrawSize(FVector2D(100, 50));

	HealthChangeDelay = .75f;
}

void UHealthAndStatusWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	auto world = GetWorld();
	auto controller = world->GetFirstPlayerController();
	auto pawn = controller->GetPawn();

	if (pawn)
	{
		CameraComponent = pawn->GetComponentByClass<UCameraComponent>();
		if (CameraComponent)
			SetComponentTickEnabled(true);
	}
	auto statusWidget = GetHealthWidget();
	statusWidget->GetArmourBarArmsDynamic()->SetPercent(0);
	statusWidget->GetArmourBarLegsDynamic()->SetPercent(0);
	statusWidget->GetArmourBarShoulderDynamic()->SetPercent(0);
	statusWidget->GetArmourBarChestDynamic()->SetPercent(0);
	statusWidget->GetArmourBarHelmetDynamic()->SetPercent(0);

	UpdateStatusWidgetData();
}
#pragma optimize("", off)
void UHealthAndStatusWidgetComponent::UpdateStatusWidgetData()
{
	auto statusWidget = GetHealthWidget();

	auto characterComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (characterComponent)
	{
		CurrentHealth = characterComponent->GetHealth();
		MaxHealth = characterComponent->GetMaxHealth();

		statusWidget->GetHealthBar()->SetPercent(characterComponent->GetHealthAsPercentage());

		UDebugMessages::LogDisplay(this, "CrewName" + characterComponent->GetCharacterName(), SVLogHealthAndStatusWidgetComponent);
		statusWidget->GetNameTag()->SetText(FText::FromString(characterComponent->GetCharacterName()));
	}

	auto equipmentComponent = GetOwner()->GetComponentByClass<UEquipmentComponent>();

	MaxArmArmour = 0;
	MaxLegArmour = 0;
	MaxShoulderArmour = 0;
	MaxChestArmour = 0;
	MaxHelmetArmour = 0;

	bUseHelmetArmourBar = false;
	bUseLegArmourBar = false;
	bUseShoulderArmourBar = false;
	bUseChestArmourBar = false;
	bUseArmArmourBar = false;

	TArray<UProgressBar *> progressBarsToUse;
	TArray<UProgressBar *> dynamicProgressBarsToUse;
	if (equipmentComponent)
	{
		for (auto clothing : equipmentComponent->GetAllClothing())
		{
			auto clothingType = clothing->GetClothingType();
			auto stats = clothing->GetComponentByClass<UClothingStatsComponent>();
			switch (clothingType)
			{
			case EClothingType::CT_Arms:
			{
				MaxArmArmour = stats->GetHealth();
				ArmArmour = stats->GetMaxHealth();
				bUseArmArmourBar = true;
				progressBarsToUse.Emplace(statusWidget->GetArmourBarArms());
				dynamicProgressBarsToUse.Emplace(statusWidget->GetArmourBarArmsDynamic());

				statusWidget->GetArmourBarArms()->SetPercent(GetPercentageOfHealth(ArmArmour, MaxArmArmour));
			}
			break;
			case EClothingType::CT_Legs:
			{
				MaxLegArmour = stats->GetHealth();
				LegArmour = stats->GetMaxHealth();
				bUseLegArmourBar = true;
				progressBarsToUse.Emplace(statusWidget->GetArmourBarLegs());
				dynamicProgressBarsToUse.Emplace(statusWidget->GetArmourBarLegsDynamic());

				statusWidget->GetArmourBarLegs()->SetPercent(GetPercentageOfHealth(LegArmour, MaxLegArmour));
			}
			break;
			case EClothingType::CT_Shoulders:
			{
				MaxShoulderArmour = stats->GetHealth();
				ShoulderArmour = stats->GetMaxHealth();
				bUseShoulderArmourBar = true;
				progressBarsToUse.Emplace(statusWidget->GetArmourBarShoulder());
				dynamicProgressBarsToUse.Emplace(statusWidget->GetArmourBarShoulderDynamic());

				statusWidget->GetArmourBarShoulder()->SetPercent(GetPercentageOfHealth(ShoulderArmour, MaxShoulderArmour));
			}
			break;
			case EClothingType::CT_Chest:
			{
				MaxChestArmour = stats->GetHealth();
				ChestArmour = stats->GetMaxHealth();
				bUseChestArmourBar = true;
				progressBarsToUse.Emplace(statusWidget->GetArmourBarChest());
				dynamicProgressBarsToUse.Emplace(statusWidget->GetArmourBarChestDynamic());

				statusWidget->GetArmourBarChest()->SetPercent(GetPercentageOfHealth(ChestArmour, MaxChestArmour));
			}
			break;
			case EClothingType::CT_Helmet:
			{
				MaxHelmetArmour = stats->GetHealth();
				HelmetArmour = stats->GetMaxHealth();
				bUseHelmetArmourBar = true;
				progressBarsToUse.Emplace(statusWidget->GetArmourBarHelmet());
				dynamicProgressBarsToUse.Emplace(statusWidget->GetArmourBarHelmetDynamic());

				statusWidget->GetArmourBarHelmet()->SetPercent(GetPercentageOfHealth(HelmetArmour, MaxHelmetArmour));

				UDebugMessages::LogDisplay(this, "Helmet Health: " + FString::FromInt(HelmetArmour) + " / " + FString::FromInt(MaxHelmetArmour), SVLogHealthAndStatusWidgetComponent);
			}
			break;
			}
		}
	}

	auto nextValidPosition = 0;
	nextValidPosition = DetermineArmourWidgetGridLocations(bUseHelmetArmourBar, statusWidget->GetArmourBarHelmet(), statusWidget->GetArmourBarHelmetDynamic(), nextValidPosition);
	nextValidPosition = DetermineArmourWidgetGridLocations(bUseLegArmourBar, statusWidget->GetArmourBarLegs(), statusWidget->GetArmourBarLegsDynamic(), nextValidPosition);
	nextValidPosition = DetermineArmourWidgetGridLocations(bUseShoulderArmourBar, statusWidget->GetArmourBarShoulder(), statusWidget->GetArmourBarShoulderDynamic(), nextValidPosition);
	nextValidPosition = DetermineArmourWidgetGridLocations(bUseChestArmourBar, statusWidget->GetArmourBarChest(), statusWidget->GetArmourBarChestDynamic(), nextValidPosition);
	nextValidPosition = DetermineArmourWidgetGridLocations(bUseArmArmourBar, statusWidget->GetArmourBarArms(), statusWidget->GetArmourBarArmsDynamic(), nextValidPosition);
}
#pragma optimize("", on)
void UHealthAndStatusWidgetComponent::SetName(FString name)
{
	auto statusWidget = GetHealthWidget();
	auto nameTag = statusWidget->GetNameTag();
	nameTag->SetText(FText::FromString(name));
}
void UHealthAndStatusWidgetComponent::SetPercentage(float value)
{
	auto statusWidget = GetHealthWidget();
	auto progress = statusWidget->GetHealthBar();
	progress->SetPercent(value);
}

void UHealthAndStatusWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CameraComponent)
		SetWorldRotation(UGridUtilities::FindLookAtRotation(GetComponentLocation(), CameraComponent->GetComponentLocation()));
}

void UHealthAndStatusWidgetComponent::UpdateOnHealthChange()
{
	auto characterComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (!characterComponent)
		return UDebugMessages::LogError(this, "failed to get character component, cannot update health and status component", SVLogHealthAndStatusWidgetComponent);

	UpdateProgressBarOnHealthChange(characterComponent->GetHealth(), characterComponent->GetMaxHealth(), GetHealthWidget()->GetHealthBar(), GetHealthWidget()->GetDynamicHealthBar());

	auto equipmentComponent = GetOwner()->GetComponentByClass<UEquipmentComponent>();
	if (!equipmentComponent)
		return UDebugMessages::LogError(this, "failed to get equipment component, cannot update health and status component", SVLogHealthAndStatusWidgetComponent);

	for (auto clothing : equipmentComponent->GetAllClothing())
	{
		auto clothingType = clothing->GetClothingType();
		auto stats = clothing->GetComponentByClass<UClothingStatsComponent>();
		switch (clothingType)
		{
		case EClothingType::CT_Arms:
		{
			UpdateProgressBarOnHealthChange(stats->GetHealth(), MaxArmArmour, GetHealthWidget()->GetArmourBarArms(), GetHealthWidget()->GetArmourBarArmsDynamic());
		}
		break;
		case EClothingType::CT_Legs:
		{
			UpdateProgressBarOnHealthChange(stats->GetHealth(), MaxLegArmour, GetHealthWidget()->GetArmourBarLegs(), GetHealthWidget()->GetArmourBarLegsDynamic());
		}
		break;
		case EClothingType::CT_Shoulders:
		{
			UpdateProgressBarOnHealthChange(stats->GetHealth(), MaxShoulderArmour, GetHealthWidget()->GetArmourBarShoulder(), GetHealthWidget()->GetArmourBarShoulderDynamic());
		}
		break;
		case EClothingType::CT_Chest:
		{
			UpdateProgressBarOnHealthChange(stats->GetHealth(), MaxChestArmour, GetHealthWidget()->GetArmourBarChest(), GetHealthWidget()->GetArmourBarChestDynamic());
		}
		break;
		case EClothingType::CT_Helmet:
		{
			UpdateProgressBarOnHealthChange(stats->GetHealth(), MaxHelmetArmour, GetHealthWidget()->GetArmourBarHelmet(), GetHealthWidget()->GetArmourBarHelmetDynamic());
		}
		break;
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(InternalHealthChangeTimer);
	GetWorld()->GetTimerManager().SetTimer(RecentHealthChangeTimer, this, &UHealthAndStatusWidgetComponent::RecentHealthChangeCallback, HealthChangeDelay, false);

	// auto newHealth = characterComponent->GetHealth();
	// auto healthDifference = CurrentHealth - newHealth;

	// auto statusWidget = GetHealthWidget();
	// auto dynamicProgressBar = statusWidget->GetDynamicHealthBar();
	// auto healthProgressBar = statusWidget->GetHealthBar();

	// if (healthDifference > 1 && RecentHealthChange != healthDifference && healthProgressBar->GetPercent() > 0)
	// {
	// 	RecentHealthChange = healthDifference;

	// 	GetWorld()->GetTimerManager().ClearTimer(InternalHealthChangeTimer);
	// 	GetWorld()->GetTimerManager().SetTimer(RecentHealthChangeTimer, this, &UHealthAndStatusWidgetComponent::RecentHealthChangeCallback, HealthChangeDelay, false);

	// 	float healthDiffPercent = GetPercentageOfHealth(healthDifference, MaxHealth);
	// 	float currentHealthPercent = GetPercentageOfHealth(CurrentHealth, MaxHealth);
	// 	float newHealthPercent = GetPercentageOfHealth(newHealth, MaxHealth);

	// 	dynamicProgressBar->SetVisibility(ESlateVisibility::Visible);
	// 	dynamicProgressBar->SetPercent(1);
	// 	healthProgressBar->SetPercent(newHealthPercent);

	// 	auto fullWidth = healthProgressBar->GetCachedGeometry().Size;
	// 	float differenceWidth = fullWidth.X * healthDiffPercent;
	// 	float newHealthWidth = fullWidth.X * newHealthPercent;

	// 	auto widgetTransform = FWidgetTransform(healthProgressBar->GetRenderTransform());
	// 	widgetTransform.Scale = FVector2D(.5f, 1);

	// 	if (auto cPanelSlot = Cast<UGridSlot>(dynamicProgressBar->Slot))
	// 	{
	// 		UDebugMessages::LogDisplay(GetOwner(), cPanelSlot->GetName(), SVLogHealthAndStatusWidgetComponent);

	// 		auto baseMargin = 0.0f;

	// 		FMargin margin = FMargin(cPanelSlot->GetPadding());

	// 		margin.Left = baseMargin + newHealthWidth;
	// 		margin.Right = baseMargin + (fullWidth.X - (newHealthWidth + differenceWidth));

	// 		cPanelSlot->SetPadding(margin);
	// 	}
	// }
	// else
	// {
	// 	float newHealthPercent = GetPercentageOfHealth(newHealth, MaxHealth);
	// 	healthProgressBar->SetPercent(newHealthPercent);
	// }
}

float UHealthAndStatusWidgetComponent::UpdateProgressBarOnHealthChange(float healthValue, float maxHealthValue, UProgressBar *progressBar, UProgressBar *dynamicProgressBar)
{
	if (!progressBar || !dynamicProgressBar)
		return 0;

	float newHealthPercent = maxHealthValue > 0 ? GetPercentageOfHealth(healthValue, maxHealthValue) : 0.0f;
	float currentHealthPercent = progressBar->GetPercent();
	float healthDifference = (currentHealthPercent - newHealthPercent) * maxHealthValue;

	if (maxHealthValue > 0 && healthDifference > 1)
	{
		float healthDiffPercent = currentHealthPercent - newHealthPercent;

		dynamicProgressBar->SetVisibility(ESlateVisibility::Visible);
		dynamicProgressBar->SetPercent(1);
		progressBar->SetPercent(newHealthPercent);

		float fullWidth = progressBar->GetCachedGeometry().Size.X;
		float differenceWidth = fullWidth * healthDiffPercent;
		float newHealthWidth = fullWidth * newHealthPercent;

		if (auto gridSlot = Cast<UGridSlot>(dynamicProgressBar->Slot))
		{
			FMargin margin = gridSlot->GetPadding();
			margin.Left = newHealthWidth;
			margin.Right = fullWidth - (newHealthWidth + differenceWidth);
			gridSlot->SetPadding(margin);
		}
	}
	else
	{
		progressBar->SetPercent(newHealthPercent);
	}

	return healthDifference;
}

void UHealthAndStatusWidgetComponent::InternalHealthChangeCallback()
{
	auto statusWidget = GetHealthWidget();
	auto dynamicHealthProgressBar = statusWidget->GetDynamicHealthBar();

	auto dynamicArmArmourBar = statusWidget->GetArmourBarArmsDynamic();
	auto dynamicLegArmourBar = statusWidget->GetArmourBarLegsDynamic();
	auto dynamicChestArmourBar = statusWidget->GetArmourBarChestDynamic();
	auto dynamicShoulderArmourBar = statusWidget->GetArmourBarShoulderDynamic();
	auto dynamicHelmetArmourBar = statusWidget->GetArmourBarHelmetDynamic();

	auto totalPercent = dynamicHealthProgressBar->GetPercent() +
						dynamicArmArmourBar->GetPercent() +
						dynamicLegArmourBar->GetPercent() +
						dynamicChestArmourBar->GetPercent() +
						dynamicShoulderArmourBar->GetPercent() +
						dynamicHelmetArmourBar->GetPercent();

	if (totalPercent <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(InternalHealthChangeTimer);
	}
	else
	{
		dynamicHealthProgressBar->SetPercent(SetZeroIfBelow(dynamicHealthProgressBar->GetPercent()));
		dynamicArmArmourBar->SetPercent(SetZeroIfBelow(dynamicArmArmourBar->GetPercent()));
		dynamicLegArmourBar->SetPercent(SetZeroIfBelow(dynamicLegArmourBar->GetPercent()));
		dynamicChestArmourBar->SetPercent(SetZeroIfBelow(dynamicChestArmourBar->GetPercent()));
		dynamicShoulderArmourBar->SetPercent(SetZeroIfBelow(dynamicShoulderArmourBar->GetPercent()));
		dynamicHelmetArmourBar->SetPercent(SetZeroIfBelow(dynamicHelmetArmourBar->GetPercent()));
	}
}

float UHealthAndStatusWidgetComponent::SetZeroIfBelow(float value)
{
	if (value - 0.01f < 0)
		return 0;
	else
		return value - 0.01f;
}

void UHealthAndStatusWidgetComponent::RecentHealthChangeCallback()
{
	auto characterComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (characterComponent)
	{
		CurrentHealth = characterComponent->GetHealth();
	}
	RecentHealthChange = 0;
	RecentArmArmourChange = 0;
	RecentShoulderArmourChange = 0;
	RecentLegArmourChange = 0;
	RecentChestArmourChange = 0;
	RecentHelmetArmourChange = 0;
	GetWorld()->GetTimerManager().SetTimer(InternalHealthChangeTimer, this, &UHealthAndStatusWidgetComponent::InternalHealthChangeCallback, 0.01f, true);
}

float UHealthAndStatusWidgetComponent::GetPercentageOfHealth(float value, float maxValue)
{
	return value / maxValue;
}

UHealthAndStatusWidget *UHealthAndStatusWidgetComponent::GetHealthWidget()
{
	return (UHealthAndStatusWidget *)GetWidget();
}

void UHealthAndStatusWidgetComponent::TakenDamage(int amount)
{
	auto statusWidget = GetHealthWidget();

	if (TotalTakenDamage == 0)
	{
		TotalTakenDamage = amount;
		statusWidget->PlayAnimationForward(statusWidget->GetAnimateDamageIn());
	}
	else
		TotalTakenDamage += amount;

	auto damageText = statusWidget->GetDamageText();
	damageText->SetText(FText::FromString(FString::SanitizeFloat(TotalTakenDamage, 0) + " Damage"));

	GetWorld()->GetTimerManager().SetTimer(DamageTakenResetTimer, this, &UHealthAndStatusWidgetComponent::OnDamageResetTimerCallback, 3.0f, false);
}

void UHealthAndStatusWidgetComponent::OnDamageResetTimerCallback()
{
	auto statusWidget = GetHealthWidget();

	auto damageText = statusWidget->GetDamageText();
	damageText->SetText(FText::FromString(""));

	statusWidget->PlayAnimationForward(statusWidget->GetAnimateDamageOut());
	TotalTakenDamage = 0;
}

void UHealthAndStatusWidgetComponent::TakenStatusEffect(FString status)
{
	auto statusWidget = GetHealthWidget();

	auto statusText = statusWidget->GetStatusText();
	statusText->SetText(FText::FromString(status));

	GetWorld()->GetTimerManager().SetTimer(StatusEffectResetTimer, this, &UHealthAndStatusWidgetComponent::OnStatusEffectTimerCallback, 3.0f, false);
}

void UHealthAndStatusWidgetComponent::OnStatusEffectTimerCallback()
{
	auto statusWidget = GetHealthWidget();

	auto statusText = statusWidget->GetStatusText();
	statusText->SetText(FText::FromString(""));

	statusWidget->PlayAnimationForward(statusWidget->GetAnimateStatusOut());
}

void UHealthAndStatusWidgetComponent::HideComponent()
{
	GetHealthWidget()->SetVisibility(ESlateVisibility::Hidden);
}
void UHealthAndStatusWidgetComponent::ShowComponent()
{
	GetHealthWidget()->SetVisibility(ESlateVisibility::Visible);
}

int UHealthAndStatusWidgetComponent::DetermineArmourWidgetGridLocations(bool bIsValidBar, UProgressBar *progressBar, UProgressBar *dynamicBar, int nextValidGridLocation)
{
	if (bIsValidBar)
	{
		if (auto gridSlot = Cast<UGridSlot>(progressBar->Slot))
			gridSlot->SetColumn(nextValidGridLocation);

		if (auto gridSlot = Cast<UGridSlot>(dynamicBar->Slot))
			gridSlot->SetColumn(nextValidGridLocation);

		nextValidGridLocation += 1;

		progressBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		progressBar->SetVisibility(ESlateVisibility::Hidden);
		dynamicBar->SetVisibility(ESlateVisibility::Hidden);
	}

	return nextValidGridLocation;
}