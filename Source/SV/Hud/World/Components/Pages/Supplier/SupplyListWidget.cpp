// Fill out your copyright notice in the Description page of Project Settings.


#include "SupplyListWidget.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/ScrollBox.h"

#include "../../../../../Utilities/SvUtilities.h"
#include "../../../../../Instance/SvGameInstance.h"
#include "../../../../../Instance/Managers/SupplyDataManager.h"
#include "../../../../../Instance/Managers/WeaponDataManager.h"
#include "../../../../../Instance/Managers/CurrentGameDataManager.h"
#include "../../../../../Instance/Managers/ThrowableDataManager.h"

#include "../../../../Helpers/UserWidgetHelpers.h"

#include "SubComponents/SupplierListItemWidget.h"

void USupplyListWidget::FillSupplyList() {
	if (!SupplyScroll)
		return UDebugMessages::LogError(this, "failed to get supply scroll");


	SupplyScroll->ClearChildren();

	auto instance = USvUtilities::GetGameInstance(GetWorld());

	if (!instance)
		return UDebugMessages::LogError(this, "failed to get game instance");

	auto supplyDataManager = instance->GetSupplyDataManager();
	auto currentGameDataManager = instance->GetCurrentGameDataManager();
	auto currentGameData = currentGameDataManager->GetCurrentGameData();
	auto throwableDataManager = instance->GetThrowableDataManager();
	auto throwableData = throwableDataManager->GetThrowableData();
	auto supplyData = supplyDataManager->GetSupplyStockData();
	auto stockData = supplyData->GetStockItems();

	auto weaponData = instance->GetWeaponDataManager()->GetWeaponData();

	UDebugMessages::LogDisplay(this, "starting stockdata " + FString::SanitizeFloat(stockData.Num(), 0));
	for (int i = 0; i < stockData.Num(); i++) {
		auto newWidget = CreateSupplyListItemWidget();
		if (newWidget)
		{
			SupplyScroll->AddChild(newWidget);
			TMap<EResourceType, int> costs;
			int amountOwned = 0;
			FString name = "Undefined";
			FString desc = "Undefined";
			UTexture2D* img = nullptr;
			bool bShortedImage = false;

			if (stockData[i].GetGunType() != EGun::INVALID) {
				auto primaries = currentGameData->GetCrewPrimaries();

				for (int x = 0; x < primaries.Num(); x++)
					if (primaries[x].GetPrimaryGunType() == stockData[i].GetGunType())
						amountOwned += 1;

				auto thisWeaponData = weaponData->GetGunWeapon(stockData[i].GetGunType());
				img = UUserWidgetHelpers::GetTextureForGun(stockData[i].GetGunType());
				name = thisWeaponData->GetName();
				desc = thisWeaponData->GetDescription();
			}

			if (stockData[i].GetThrowableType() != EThrowable::INVALID) {
				auto tools = currentGameData->GetAllTools();

				for (int x = 0; x < tools.Num(); x++)
					if (tools[x].GetToolType() == EToolType::TT_Throwable && tools[x].GetTool() == (uint8)stockData[i].GetThrowableType())
						amountOwned += 1;

				auto throwable = throwableData->GetThrowableByType(stockData[i].GetThrowableType());
				name = throwable->GetName();
				desc = throwable->GetDescription();
				img = UUserWidgetHelpers::GetTextureForTool(EToolType::TT_Throwable, (uint8)stockData[i].GetThrowableType());
				bShortedImage = true;
			}


			newWidget->SetNameOfItem(name);
			newWidget->SetBigImage(img);
			newWidget->SetDescriptionOfItem(desc);
			newWidget->SetAmountOwnedText(amountOwned);
			newWidget->AddCosts(stockData[i].GetCosts());
			newWidget->SetStockItem(&stockData[i]);

			if (bShortedImage)
				newWidget->ShortenImage();
		}
	}
}

USupplierListItemWidget* USupplyListWidget::CreateSupplyListItemWidget() {
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/World/Components/Pages/Supplies/SubComponents/SupplyListItem_Bp.SupplyListItem_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<USupplierListItemWidget>())
		return CreateWidget<USupplierListItemWidget>(this, hudUIWidgetClass);

	return nullptr;
}