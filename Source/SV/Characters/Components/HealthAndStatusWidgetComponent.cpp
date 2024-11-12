// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAndStatusWidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Utilities/GridUtilities.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/GridSlot.h"
#include "Animation/WidgetAnimation.h"
#include "../../Hud/CharacterWidgets/HealthAndStatusWidget.h"

UHealthAndStatusWidgetComponent::UHealthAndStatusWidgetComponent() {
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Characters/Widgets/HealthAndStatusWidgetComponent_Bp.HealthAndStatusWidgetComponent_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UUserWidget>())
	{
		WidgetClass = hudUIWidgetClass;
	}
	SetDrawSize(FVector2D(100, 50));

	HealthChangeDelay = .75f;
}

void UHealthAndStatusWidgetComponent::BeginPlay() {
	Super::BeginPlay();
	auto world = GetWorld();
	auto controller = world->GetFirstPlayerController();
	auto pawn = controller->GetPawn();

	if (pawn) {
		CameraComponent = pawn->GetComponentByClass<UCameraComponent>();
		if (CameraComponent)
			SetComponentTickEnabled(true);
	}

	auto statusWidget = GetHealthWidget();

	auto characterComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (characterComponent) {
		CurrentHealth = characterComponent->GetHealth();
		MaxHealth = characterComponent->GetMaxHealth();

		statusWidget->GetHealthBar()->SetPercent(characterComponent->GetHealthAsPercentage());
		statusWidget->GetNameTag()->SetText(FText::FromString(characterComponent->GetCharacterName()));
	}
}

void UHealthAndStatusWidgetComponent::SetName(FString name) {
	auto statusWidget = GetHealthWidget();
	auto nameTag = statusWidget->GetNameTag();
	nameTag->SetText(FText::FromString(name));
}
void UHealthAndStatusWidgetComponent::SetPercentage(float value) {
	auto statusWidget = GetHealthWidget();
	auto progress = statusWidget->GetHealthBar();
	progress->SetPercent(value);
}

void UHealthAndStatusWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CameraComponent)
		SetWorldRotation(UGridUtilities::FindLookAtRotation(GetComponentLocation(), CameraComponent->GetComponentLocation()));
}

void UHealthAndStatusWidgetComponent::UpdateOnHealthChange() {
	auto characterComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (!characterComponent)
		return UDebugMessages::LogError(this, "failed to get character component, cannot update health and status component");

	auto newHealth = characterComponent->GetHealth();
	auto healthDifference = CurrentHealth - newHealth;

	auto statusWidget = GetHealthWidget();
	auto dynamicProgressBar = statusWidget->GetDynamicHealthBar();
	auto healthProgressBar = statusWidget->GetHealthBar();

	if (healthDifference > 1 && RecentHealthChange != healthDifference && healthProgressBar->GetPercent() > 0) {
		RecentHealthChange = healthDifference;

		GetWorld()->GetTimerManager().ClearTimer(InternalHealthChangeTimer);
		GetWorld()->GetTimerManager().SetTimer(RecentHealthChangeTimer, this, &UHealthAndStatusWidgetComponent::RecentHealthChangeCallback, HealthChangeDelay, false);

		float healthDiffPercent = GetPercentageOfHealth(healthDifference, MaxHealth);
		float currentHealthPercent = GetPercentageOfHealth(CurrentHealth, MaxHealth);
		float newHealthPercent = GetPercentageOfHealth(newHealth, MaxHealth);

		dynamicProgressBar->SetVisibility(ESlateVisibility::Visible);
		dynamicProgressBar->SetPercent(1);
		healthProgressBar->SetPercent(newHealthPercent);

		auto fullWidth = healthProgressBar->GetCachedGeometry().Size;
		float differenceWidth = fullWidth.X * healthDiffPercent;
		float newHealthWidth = fullWidth.X * newHealthPercent;

		auto widgetTransform = FWidgetTransform(healthProgressBar->GetRenderTransform());
		widgetTransform.Scale = FVector2D(.5f, 1);

		if (auto cPanelSlot = Cast<UGridSlot>(dynamicProgressBar->Slot)) {
			UDebugMessages::LogDisplay(GetOwner(), cPanelSlot->GetName());

			auto baseMargin = 0.0f;

			FMargin margin = FMargin(cPanelSlot->GetPadding());

			margin.Left = baseMargin + newHealthWidth;
			margin.Right = baseMargin + (fullWidth.X - (newHealthWidth + differenceWidth));

			cPanelSlot->SetPadding(margin);
		}
	}
	else {
		float newHealthPercent = GetPercentageOfHealth(newHealth, MaxHealth);
		healthProgressBar->SetPercent(newHealthPercent);
	}
}



void UHealthAndStatusWidgetComponent::InternalHealthChangeCallback() {
	auto statusWidget = GetHealthWidget();
	auto dynamicProgressBar = statusWidget->GetDynamicHealthBar();

	if (dynamicProgressBar->GetPercent() <= 0) {
		GetWorld()->GetTimerManager().ClearTimer(InternalHealthChangeTimer);
	}
	else {
		dynamicProgressBar->SetPercent(dynamicProgressBar->GetPercent() - 0.01f);
	}
}
void UHealthAndStatusWidgetComponent::RecentHealthChangeCallback() {
	auto characterComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (characterComponent) {
		CurrentHealth = characterComponent->GetHealth();
	}
	RecentHealthChange = 0;
	GetWorld()->GetTimerManager().SetTimer(InternalHealthChangeTimer, this, &UHealthAndStatusWidgetComponent::InternalHealthChangeCallback, 0.01f, true);
}

float UHealthAndStatusWidgetComponent::GetPercentageOfHealth(float value, float maxValue) {
	return value / maxValue;
}

UHealthAndStatusWidget* UHealthAndStatusWidgetComponent::GetHealthWidget() {
	return (UHealthAndStatusWidget*)GetWidget();
}

void UHealthAndStatusWidgetComponent::TakenDamage(int amount) {
	auto statusWidget = GetHealthWidget();

	if (TotalTakenDamage == 0) {
		TotalTakenDamage = amount;
		statusWidget->PlayAnimationForward(statusWidget->GetAnimateDamageIn());
	}
	else TotalTakenDamage += amount;

	auto damageText = statusWidget->GetDamageText();
	damageText->SetText(FText::FromString(FString::SanitizeFloat(TotalTakenDamage, 0) + " Damage"));

	GetWorld()->GetTimerManager().SetTimer(DamageTakenResetTimer, this, &UHealthAndStatusWidgetComponent::OnDamageResetTimerCallback, 3.0f, false);
}

void UHealthAndStatusWidgetComponent::OnDamageResetTimerCallback() {
	auto statusWidget = GetHealthWidget();

	auto damageText = statusWidget->GetDamageText();
	damageText->SetText(FText::FromString(""));

	statusWidget->PlayAnimationForward(statusWidget->GetAnimateDamageOut());
	TotalTakenDamage = 0;
}

void UHealthAndStatusWidgetComponent::TakenStatusEffect(FString status) {
	auto statusWidget = GetHealthWidget();

	auto statusText = statusWidget->GetStatusText();
	statusText->SetText(FText::FromString(status));

	GetWorld()->GetTimerManager().SetTimer(StatusEffectResetTimer, this, &UHealthAndStatusWidgetComponent::OnStatusEffectTimerCallback, 3.0f, false);
}

void UHealthAndStatusWidgetComponent::OnStatusEffectTimerCallback() {
	auto statusWidget = GetHealthWidget();

	auto statusText = statusWidget->GetStatusText();
	statusText->SetText(FText::FromString(""));

	statusWidget->PlayAnimationForward(statusWidget->GetAnimateStatusOut());
}