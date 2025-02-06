// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterTileWidget.h"
#include "../../../Delegates/HudDelegates.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/HorizontalBox.h"
#include "SubComponents/CharacterTileItemWidget.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Player/Managers/SelectionManager.h"
#include "../../../Interfaces/Selectable.h"

void UCharacterTileWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hudDelegates");

	hudDelegates->_AddSoldierToCharacterTileWidget.AddDynamic(this, &UCharacterTileWidget::RecieveCharacter);
	hudDelegates->_SelectNextCharacterWithAp.AddDynamic(this, &UCharacterTileWidget::AttemptToSelectNextUnitWithAP);

	hudDelegates->_CharacterTileVisibility.AddDynamic(this, &UCharacterTileWidget::SetCharacterTileVisibility);
}

void UCharacterTileWidget::RecieveCharacter(AActor* actor) {

	auto horizontalBox = (UHorizontalBox*)GetWidgetFromName("HorizontalBox");
	if (horizontalBox) {
		auto createdWidget = CreateItemWidget();
		if (!createdWidget)
			return UDebugMessages::LogError(this, "failed to get created widget");

		createdWidget->SetRepresentedActor(actor);
		horizontalBox->AddChildToHorizontalBox(createdWidget);
	}
}

void UCharacterTileWidget::AttemptToSelectNextUnitWithAP() {
	auto horizontalBox = (UHorizontalBox*)GetWidgetFromName("HorizontalBox");

	auto controller = GetWorld()->GetFirstPlayerController();
	auto selectionManager = controller->GetComponentByClass<USelectionManager>();

	auto selected = selectionManager->GetSelected();
	bool foundCurrentSelected = false;

	if (!selected) foundCurrentSelected = true;

	if (horizontalBox) {
		auto children = horizontalBox->GetAllChildren();

		//if the current selected is the last in the list, 
		//just set as true as to start again and select the first
		if (selected && children.Num() > 0) {
			auto last = (UCharacterTileItemWidget*)children[children.Num() - 1];
			if (last->GetRepresentedActor() == selected->GetAsActor())
				foundCurrentSelected = true;
		}

		for (int i = 0; i < children.Num(); i++) {
			UCharacterTileItemWidget* item = (UCharacterTileItemWidget*)children[i];

			auto actor = item->GetRepresentedActor();
			if (!actor)
				continue;

			if (selected && selected->GetAsActor() == actor) {
				foundCurrentSelected = true;
				continue;
			}

			if (!foundCurrentSelected) continue;

			auto details = actor->GetComponentByClass<UCharacterDetailsComponent>();
			if (details) {
				auto ap = details->GetActionPoints();
				if (ap > 0) {
					item->SimulateOnCharacterButtonClicked(item->GetRepresentedActor());
					break;
				}
			}
		}
	}
}

UCharacterTileItemWidget* UCharacterTileWidget::CreateItemWidget() {
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/Components/SubComponents/CharacterTileItemWidget_Bp.CharacterTileItemWidget_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UCharacterTileItemWidget>())
		return CreateWidget<UCharacterTileItemWidget>(this, hudUIWidgetClass);

	return nullptr;
}

void UCharacterTileWidget::SetCharacterTileVisibility(bool bVisible) {
	if (bVisible) SetVisibility(ESlateVisibility::Visible);
	else SetVisibility(ESlateVisibility::Hidden);
}