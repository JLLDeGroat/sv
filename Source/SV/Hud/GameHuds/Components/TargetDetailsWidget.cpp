// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDetailsWidget.h"
#include "../../../Delegates/HudDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "SubComponents/TargetDetailsRowWidget.h"
#include "SubComponents/TargetDetailsRowItemWidget.h"
#include "../../../Player/GamePlayerController.h"
#include "../../../Player/Actions/TargetAction.h"
#include "../../../Player/Managers/SelectionManager.h"
#include "../../../Interfaces/Selectable.h"
#include "../../../Interfaces/SvChar.h"
#include "../../../Characters/Components/TargetingComponent.h"
#include "../../../Player/PlayerPawn.h"
#include "../../../Player/Components/PawnCameraComponent.h"
#include "Components/Button.h"
#include "../../Helpers/UserWidgetHelpers.h"

void UTargetDetailsWidget::NativeConstruct() {
	auto hudDelegates = UHudDelegates::GetInstance();

	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not find hud instances");

	hudDelegates->_AddTargetDataToHud.AddDynamic(this, &UTargetDetailsWidget::OnAddTargetData);
	hudDelegates->_ClearTargetDataHud.AddDynamic(this, &UTargetDetailsWidget::OnClearTargetData);
	hudDelegates->_TargetIconClicked.AddDynamic(this, &UTargetDetailsWidget::OnTargetIconClicked);

	hudDelegates->_HideOrResetUIWidget.AddDynamic(this, &UTargetDetailsWidget::HideOrReset);
	hudDelegates->_CycleToNextTarget.AddDynamic(this, &UTargetDetailsWidget::CycleTarget);
}

void UTargetDetailsWidget::OnAddTargetData(FGuid Id, FVector SourceLocation, FVector TargetLocation, ETargetIcon TargetIcon) {
	UDebugMessages::LogDisplay(this, "adding target data to hud " + Id.ToString());

	auto verticalDetailsBox = GetDetailsBox();
	if (!verticalDetailsBox) return UDebugMessages::LogError(this, "failed to get vertical details box");

	UHorizontalBox* lastHorizontalBox = GetLatestHorizontalBox(verticalDetailsBox);

	auto newHorizontalRowItem = CreateTargetDetailsRowItemWidget();
	newHorizontalRowItem->SetSourceLocation(SourceLocation);
	newHorizontalRowItem->SetTargetLocation(TargetLocation);
	newHorizontalRowItem->SetId(Id);

	auto imageTexture = UUserWidgetHelpers::GetTargetIcon(TargetIcon);
	newHorizontalRowItem->SetImage(imageTexture);

	if (!lastHorizontalBox || lastHorizontalBox->GetChildrenCount() >= 3) {
		auto newHorizontalWidget = CreateTargetDetailsRowWidget();

		if (!newHorizontalWidget)
			return UDebugMessages::LogError(this, "failed to create new horizontal box");

		if (!lastHorizontalBox) {
			auto itemWidgetButton = newHorizontalRowItem->GetItemButton();
			itemWidgetButton->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0, 175, 175, .4f));
			//itemWidgetButton->WidgetStyle.Normal.OutlineSettings.Color = FSlateColor(FLinearColor(0, 55, 255, 1));
		}

		verticalDetailsBox->AddChildToVerticalBox(newHorizontalWidget);
		lastHorizontalBox = GetHorizontalBoxFromWidget(newHorizontalWidget);// ->AddChildToHorizontalBox(newHorizontalRowItem);
	}

	lastHorizontalBox->AddChildToHorizontalBox(newHorizontalRowItem);
}

void UTargetDetailsWidget::OnClearTargetData() {
	UDebugMessages::LogDisplay(this, "clearing target data hud");
	auto targetDataBox = GetDetailsBox();
	if (targetDataBox)
		targetDataBox->ClearChildren();
}

void UTargetDetailsWidget::OnTargetIconClicked(FGuid Id, FVector Location) {
	auto detailBox = GetDetailsBox();
	auto detailBoxChildren = detailBox->GetAllChildren();
	for (int i = 0; i < detailBoxChildren.Num(); i++) {

		auto horizontalBox = GetHorizontalBoxFromWidget((UUserWidget*)detailBoxChildren[i]);
		auto allHorizontalChildren = horizontalBox->GetAllChildren();
		for (int x = 0; x < allHorizontalChildren.Num(); x++) {
			auto itemWidget = (UTargetDetailsRowItemWidget*)allHorizontalChildren[x];
			auto itemWidgetButton = itemWidget->GetItemButton();

			if (itemWidget->GetId() == Id) {
				itemWidgetButton->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0, 175, 175, .4f));
				//itemWidgetButton->WidgetStyle.Normal.OutlineSettings.Color = FSlateColor(FLinearColor(0, 55, 255, 1));
			}
			else {
				itemWidgetButton->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0, 175, 175, 0));
				//itemWidgetButton->WidgetStyle.Normal.OutlineSettings.Color = FSlateColor(FLinearColor(0, 55, 255, 0));
			}
		}
	}

	auto playerController = GetWorld()->GetFirstPlayerController<AGamePlayerController>();
	if (!playerController)
		return UDebugMessages::LogError(this, "failed to get player controller, will not set targetdata");

	auto selectionManager = playerController->GetSelectionManager();
	auto selectedActor = selectionManager->GetSelected()->GetAsActor();
	if (selectedActor && selectedActor->GetComponentByClass<UTargetingComponent>()) {
		auto targetComponent = selectedActor->GetComponentByClass<UTargetingComponent>();
		targetComponent->SetCurrentMainTargetId(Id);

		auto pawn = playerController->GetPawn<APlayerPawn>();
		if (pawn && pawn->GetComponentByClass<UPawnCameraComponent>())
			pawn->GetComponentByClass<UPawnCameraComponent>()->UpdateCameraState(ECameraState::CS_ReTarget, Location);
	}
}

void UTargetDetailsWidget::HideOrReset() {
	OnClearTargetData();
}

void UTargetDetailsWidget::CycleTarget() {
	auto playerController = GetWorld()->GetFirstPlayerController<AGamePlayerController>();
	if (!playerController)
		return UDebugMessages::LogError(this, "failed to get player controller, will not set targetdata");

	auto targetAction = playerController->GetComponentByClass<UTargetAction>();
	if (!targetAction)
		return UDebugMessages::LogError(this, "Failed to get Target Action component");

	FGuid currentId = FGuid::NewGuid();
	auto selectionManager = playerController->GetSelectionManager();
	auto selectedActor = selectionManager->GetSelected()->GetAsActor();

	if (!selectedActor)
		return UDebugMessages::LogError(this, "failed to get selected actor");

	auto targetingComponent = selectedActor->GetComponentByClass<UTargetingComponent>();

	if (!targetingComponent)
		return UDebugMessages::LogError(this, "failed to get targeting component");

	targetingComponent = selectedActor->GetComponentByClass<UTargetingComponent>();
	auto currentTarget = targetingComponent->GetCurrentMainTarget();

	if (currentTarget)
		currentId = currentTarget->GetId();

	auto detailsBox = GetDetailsBox();
	auto detailChildren = detailsBox->GetAllChildren();

	TArray<UTargetDetailsRowItemWidget*> items;
	for (int i = 0; i < detailChildren.Num(); i++) {
		auto horizontalBox = GetHorizontalBoxFromWidget((UUserWidget*)detailChildren[i]);
		auto res = GetItemsFromHorizontalBox(horizontalBox);
		for (int x = 0; x < res.Num(); x++)
			items.Emplace(res[x]);
	}

	bool foundCurrent = false;
	bool foundNew = false;
	for (int i = 0; i < items.Num(); i++) {
		if (foundCurrent) {
			targetingComponent->SetCurrentMainTargetId(items[i]->GetId());
			auto mainTargetingComponent = targetingComponent->GetCurrentMainTarget();

			targetAction->SetComponentTickEnabled(true);
			playerController->SetMouseAsGame();

			auto itemWidgetButton = items[i]->GetItemButton();
			itemWidgetButton->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0, 175, 175, 1));
			itemWidgetButton->WidgetStyle.Normal.OutlineSettings.Color = FSlateColor(FLinearColor(0, 55, 255, 1));

			auto pawn = playerController->GetPawn<APlayerPawn>();

			if (pawn && pawn->GetComponentByClass<UPawnCameraComponent>()) {
				auto pawnCameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();
				if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_GunTarget)
					pawnCameraComponent->UpdateCameraState(ECameraState::CS_GunTarget, mainTargetingComponent->GetShootCameraLocation(),
						mainTargetingComponent->GetCharacter()->GetSelectableGridLocation(), false, true);
				else
					pawnCameraComponent->UpdateCameraState(ECameraState::CS_ReTarget, items[i]->GetTargetLocation());
			}
			foundNew = true;
			break;
		}

		if (items[i]->GetId() == currentId) {
			foundCurrent = true;
			auto itemWidgetButton = items[i]->GetItemButton();
			itemWidgetButton->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0, 175, 175, 0));
			itemWidgetButton->WidgetStyle.Normal.OutlineSettings.Color = FSlateColor(FLinearColor(0, 55, 255, 0));
		}
	}

	if (!foundNew && items.Num() > 0) {
		targetingComponent->SetCurrentMainTargetId(items[0]->GetId());
		auto mainTargetingComponent = targetingComponent->GetCurrentMainTarget();

		targetAction->SetComponentTickEnabled(true);
		playerController->SetMouseAsGame();

		auto itemWidgetButton = items[0]->GetItemButton();
		itemWidgetButton->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0, 175, 175, 1));
		itemWidgetButton->WidgetStyle.Normal.OutlineSettings.Color = FSlateColor(FLinearColor(0, 55, 255, 1));

		auto pawn = playerController->GetPawn<APlayerPawn>();
		if (pawn && pawn->GetComponentByClass<UPawnCameraComponent>()) {
			auto pawnCameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();
			if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_GunTarget)
				pawnCameraComponent->UpdateCameraState(ECameraState::CS_GunTarget, mainTargetingComponent->GetShootCameraLocation(),
					mainTargetingComponent->GetCharacter()->GetSelectableGridLocation(), false, true);
			else
				pawnCameraComponent->UpdateCameraState(ECameraState::CS_ReTarget, items[0]->GetTargetLocation());
		}
	}
}

UVerticalBox* UTargetDetailsWidget::GetDetailsBox() const {
	auto verticalBoxWidget = GetWidgetFromName("TargetDataBox");
	if (verticalBoxWidget->IsA<UVerticalBox>())
		return (UVerticalBox*)verticalBoxWidget;
	else
		return nullptr;
}
UHorizontalBox* UTargetDetailsWidget::GetLatestHorizontalBox(UVerticalBox* box) {
	auto childrenCount = box->GetChildrenCount();
	UWidget* lastBoxWidget = nullptr;
	if (childrenCount > 0)
		lastBoxWidget = box->GetChildAt(childrenCount - 1);

	if (lastBoxWidget) return GetHorizontalBoxFromWidget((UUserWidget*)lastBoxWidget);
	else return nullptr;
}

UHorizontalBox* UTargetDetailsWidget::GetHorizontalBoxFromWidget(UUserWidget* targetDetailsRowWidget) {
	auto widget = targetDetailsRowWidget->GetWidgetFromName("HorizontalBox");
	if (widget) return (UHorizontalBox*)widget;
	else return nullptr;
}

TArray<UTargetDetailsRowItemWidget*> UTargetDetailsWidget::GetItemsFromHorizontalBox(UHorizontalBox* box) {
	TArray<UTargetDetailsRowItemWidget*> result;
	if (!box) return result;

	auto children = box->GetAllChildren();
	for (int i = 0; i < children.Num(); i++) {
		auto thisChildAsRowItem = (UTargetDetailsRowItemWidget*)children[i];
		if (thisChildAsRowItem)
			result.Emplace(thisChildAsRowItem);
	}

	return result;
}

UTargetDetailsRowWidget* UTargetDetailsWidget::CreateTargetDetailsRowWidget() const {
	FSoftClassPath hudUIRef(TEXT("Script/UMGEditor.WidgetBlueprint'/Game/Widgets/Components/SubComponents/TargetDetailsRowComponent_Bp.TargetDetailsRowComponent_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UTargetDetailsRowWidget>())
	{
		return CreateWidget<UTargetDetailsRowWidget>(GetWorld(), hudUIWidgetClass);
	}

	return nullptr;
}

UTargetDetailsRowItemWidget* UTargetDetailsWidget::CreateTargetDetailsRowItemWidget() const {
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/Components/SubComponents/TargetDetailsRowItemWidget_Bp.TargetDetailsRowItemWidget_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UTargetDetailsRowItemWidget>())
	{
		return CreateWidget<UTargetDetailsRowItemWidget>(GetWorld(), hudUIWidgetClass);
	}

	return nullptr;
}