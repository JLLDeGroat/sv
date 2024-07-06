// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAndStatusWidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Utilities/GridUtilities.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/GridSlot.h"

UHealthAndStatusWidgetComponent::UHealthAndStatusWidgetComponent() {
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Characters/Widgets/HealthAndStatusWidgetComponent_Bp.HealthAndStatusWidgetComponent_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UUserWidget>())
	{
		WidgetClass = hudUIWidgetClass;
	}
	SetDrawSize(FVector2D(100, 50));

	HealthChangeDelay = 3.0f;
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

	auto characterComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (characterComponent) {
		CurrentHealth = characterComponent->GetHealth();
		MaxHealth = characterComponent->GetMaxHealth();

		SetPercentage(characterComponent->GetHealthAsPercentage());
		SetName(characterComponent->GetCharacterName());
	}

	HealthProgressBar = (UProgressBar*)GetWidget()->GetWidgetFromName(*PercentName);
	DynamicHealthProgressBar = (UProgressBar*)GetWidget()->GetWidgetFromName(*DynamicPercentName);
}

void UHealthAndStatusWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetWorldRotation(UGridUtilities::FindLookAtRotation(GetComponentLocation(), CameraComponent->GetComponentLocation()));
}

void UHealthAndStatusWidgetComponent::SetName(FString name) const {
	auto textBlockWidget = GetWidget()->GetWidgetFromName(*NameTag);
	UTextBlock* textBlock = (UTextBlock*)textBlockWidget;
	if (textBlock)
		textBlock->SetText(FText::FromString(name));
}

void UHealthAndStatusWidgetComponent::SetPercentage(float value) const {
	auto progressWidget = GetWidget()->GetWidgetFromName(*PercentName);
	UProgressBar* progressBar = (UProgressBar*)progressWidget;
	if (progressBar)
		progressBar->SetPercent(value);
}
#pragma optimize("", off)
void UHealthAndStatusWidgetComponent::UpdateOnHealthChange() {
	/*auto characterComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (characterComponent) {
		auto percentage = characterComponent->GetHealthAsPercentage();

		if (percentage <= 0)
			GetWidget()->SetVisibility(ESlateVisibility::Hidden);
		SetPercentage(percentage);
	}*/


	auto characterComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (!characterComponent)
		return UDebugMessages::LogError(this, "failed to get character component, cannot update health and status component");

	auto newHealth = characterComponent->GetHealth();
	auto healthDifference = CurrentHealth - newHealth;

	if (healthDifference > 1 && RecentHealthChange != healthDifference && GetCurrentPercentage() > 0) {
		RecentHealthChange = healthDifference;

		GetWorld()->GetTimerManager().ClearTimer(InternalHealthChangeTimer);
		GetWorld()->GetTimerManager().SetTimer(RecentHealthChangeTimer, this, &UHealthAndStatusWidgetComponent::RecentHealthChangeCallback, HealthChangeDelay, false);

		float healthDiffPercent = GetPercentageOfHealth(healthDifference, MaxHealth);
		float currentHealthPercent = GetPercentageOfHealth(CurrentHealth, MaxHealth);
		float newHealthPercent = GetPercentageOfHealth(newHealth, MaxHealth);

		DynamicHealthProgressBar->SetVisibility(ESlateVisibility::Visible);
		DynamicHealthProgressBar->SetPercent(1);
		HealthProgressBar->SetPercent(newHealthPercent);

		auto fullWidth = HealthProgressBar->GetCachedGeometry().Size;
		float differenceWidth = fullWidth.X * healthDiffPercent;
		float newHealthWidth = fullWidth.X * newHealthPercent;

		auto widgetTransform = FWidgetTransform(HealthProgressBar->GetRenderTransform());
		widgetTransform.Scale = FVector2D(.5f, 1);

		if (auto cPanelSlot = Cast<UGridSlot>(DynamicHealthProgressBar->Slot)) {
			UDebugMessages::LogDisplay(GetOwner(), cPanelSlot->GetName());
			
			auto baseMargin = 0.0f;
			
			FMargin margin = FMargin(cPanelSlot->GetPadding());
			
			margin.Left = baseMargin + newHealthWidth;
			margin.Right = baseMargin + (fullWidth.X - (newHealthWidth + differenceWidth));

			cPanelSlot->SetPadding(margin);
		}
	}
}



void UHealthAndStatusWidgetComponent::InternalHealthChangeCallback() {
	if (DynamicHealthProgressBar->GetPercent() <= 0) {
		GetWorld()->GetTimerManager().ClearTimer(InternalHealthChangeTimer);
	}
	else {
		DynamicHealthProgressBar->SetPercent(DynamicHealthProgressBar->GetPercent() - 0.01f);
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

float UHealthAndStatusWidgetComponent::GetCurrentPercentage() {
	auto progressWidget = GetWidget()->GetWidgetFromName(*PercentName);
	UProgressBar* progressBar = (UProgressBar*)progressWidget;
	if (progressBar)
		return progressBar->GetPercent();
	else 
		return 0.0f;
}

float UHealthAndStatusWidgetComponent::GetPercentageOfHealth(float value, float maxValue) {
	return value / maxValue;
}
#pragma optimize("", on)