// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrentResourceWidget.h"
#include "SubComponents/CurrentResourceItemWidget.h"

#include "../../../Instance/SvGameInstance.h"
#include "../../../Instance/Managers/CurrentGameDataManager.h"
#include "../../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Helpers/UserWidgetHelpers.h"
#include "Components/VerticalBox.h"
#include "../../../Delegates/HudDelegates.h"

void UCurrentResourceWidget::NativeConstruct() {
	Super::NativeConstruct();


	auto currentGameData = USvUtilities::GetGameInstance(GetWorld());
	if (!currentGameData)
		return UDebugMessages::LogError(this, "failed to get current game data");

	auto currentResources = currentGameData
		->GetCurrentGameDataManager()
		->GetCurrentGameData()
		->GetResourceData();

	if (!ResourceBox)
		return UDebugMessages::LogError(this, "failed to get current resource box");

	auto scrapRes = currentResources->GetResource(EResourceType::RT_Currency);
	AddResourceItem(EResourceType::RT_Currency, scrapRes->GetAmount());

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");

	hudDelegates->_OnResourceChange.AddDynamic(this, &UCurrentResourceWidget::OnResourceChange);
}

void UCurrentResourceWidget::AddResourceItem(EResourceType rType, int amount) {
	auto widget = CreateResourceItemWidget();
	if (widget) {
		ResourceItems.Emplace(widget);

		auto texture = UUserWidgetHelpers::GetTextureForResourceType(rType);

		widget->SetResourceImage(texture);
		widget->SetResourceText(FString::SanitizeFloat(amount, 0));
		widget->SetResourceType(rType);

		ResourceBox->AddChildToVerticalBox(widget);
	}
	else return UDebugMessages::LogError(this, "failed to create new widget");
}


UCurrentResourceItemWidget* UCurrentResourceWidget::CreateResourceItemWidget() {
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/World/Components/SubComponents/CurrentResourceItemWidget_Bp.CurrentResourceItemWidget_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UCurrentResourceItemWidget>())
		return CreateWidget<UCurrentResourceItemWidget>(this, hudUIWidgetClass);

	return nullptr;
}

void UCurrentResourceWidget::OnResourceChange(EResourceType rType) {
	for (int i = 0; i < ResourceItems.Num(); i++) {
		if (ResourceItems[i] && ResourceItems[i]->GetResourceType() == rType) {
			auto currentData = USvUtilities::GetCurrentGameData(GetWorld());
			auto resource = currentData->GetResourceData()->GetResource(rType);
			ResourceItems[i]->SetResourceText(FString::SanitizeFloat(resource->GetAmount(), 0));
		}
	}
}