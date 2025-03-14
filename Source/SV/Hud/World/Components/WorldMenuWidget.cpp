// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldMenuWidget.h"
#include "Components/Button.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Instance/SvGameInstance.h"
#include "../../../Instance/Managers/CurrentGameDataManager.h"
#include "../../../Delegates/WorldDelegates.h"
#include "../SquadPageWidget.h"
#include "../../Helpers/UserWidgetHelpers.h"

void UWorldMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto worldMapBtn = (UButton *)GetWidgetFromName("WorldMapBtn");
	if (worldMapBtn)
		worldMapBtn->OnClicked.AddDynamic(this, &UWorldMenuWidget::WorldMapClicked);
	else
		UDebugMessages::LogError(this, "failed to get world map btn");

	auto squadBtn = (UButton *)GetWidgetFromName("SquadBtn");
	if (squadBtn)
		squadBtn->OnClicked.AddDynamic(this, &UWorldMenuWidget::SquadClicked);
	else
		UDebugMessages::LogError(this, "failed to get squadBtn btn");

	auto shopBtn = (UButton *)GetWidgetFromName("ShopBtn");
	if (shopBtn)
		shopBtn->OnClicked.AddDynamic(this, &UWorldMenuWidget::SuppliesClicked);
	else
		UDebugMessages::LogError(this, "failed to get Shop btn");

	UUserWidgetHelpers::DesignButton(this, "WorldMapBtn");
	UUserWidgetHelpers::DesignButton(this, "SquadBtn");
	UUserWidgetHelpers::DesignButton(this, "ShopBtn");
}

void UWorldMenuWidget::WorldMapClicked()
{
	CloseWorldPage("SquadPage");
	CloseWorldPage("SupplyPage");
	OpenWorldPage("WorldPage");
}

void UWorldMenuWidget::SquadClicked()
{
	CloseWorldPage("SupplyPage");
	CloseWorldPage("WorldPage");
	OpenWorldPage("SquadPage");
}

void UWorldMenuWidget::SuppliesClicked()
{
	CloseWorldPage("SquadPage");
	CloseWorldPage("WorldPage");
	OpenWorldPage("SupplyPage");
}

void UWorldMenuWidget::OpenWorldPage(FString pageName)
{
	auto widget = (UUserWidget *)GetOuter()->GetOuter();
	if (widget)
	{
		auto page = widget->GetWidgetFromName(FName(pageName));
		if (page && page->Implements<UWorldPage>())
		{
			TScriptInterface<IWorldPage> worldPage = page;
			worldPage->OpenPage();

			if (pageName == "WorldPage")
			{
				auto worldInstance = UWorldDelegates::GetInstance();
				worldInstance->_OnWorldPageChange.Broadcast(true);
			}
		}
	}
}

void UWorldMenuWidget::CloseWorldPage(FString pageName)
{
	auto widget = (UUserWidget *)GetOuter()->GetOuter();
	if (widget)
	{
		auto page = widget->GetWidgetFromName(FName(pageName));
		if (page && page->Implements<UWorldPage>())
		{
			page->SetVisibility(ESlateVisibility::Hidden);
			if (pageName == "WorldPage")
			{
				auto worldInstance = UWorldDelegates::GetInstance();
				worldInstance->_OnWorldPageChange.Broadcast(false);
			}
		}
	}
}