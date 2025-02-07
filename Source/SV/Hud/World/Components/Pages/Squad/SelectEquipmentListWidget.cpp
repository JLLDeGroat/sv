// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectEquipmentListWidget.h"
#include "SelectEquipmentListItemWidget.h"
#include "../../../../../Utilities/SvUtilities.h"
#include "../../../../../Instance/SvGameInstance.h"
#include "../../../../../Instance/Managers/CurrentGameDataManager.h"
#include "../../../../Helpers/UserWidgetHelpers.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "CrewDetailsGridWidget.h"

void USelectEquipmentListWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
}

void USelectEquipmentListWidget::OpenListForPrimaries(FGuid memberId)
{
	SetVisibility(ESlateVisibility::Visible);

	CurrentMemberId = memberId;

	auto instance = USvUtilities::GetGameInstance(GetWorld());

	auto gameDataManager = instance->GetCurrentGameDataManager();
	auto gameData = gameDataManager->GetCurrentGameData();

	ScrollBox->ClearChildren();

	TArray<USelectEquipmentListItemWidget *> itemWidgets;

	auto emptyListItem = CreateListItem();
	emptyListItem->AddPrimaryDelegate();
	ScrollBox->AddChild(emptyListItem);
	// setup button on click function
	auto primaries = gameData->GetCrewPrimaries();

	for (int i = 0; i < primaries.Num(); i++)
	{
		if (primaries[i].GetCrewMemberId() == FGuid::FGuid())
		{

			bool bCreateNewItem = true;

			for (int x = 0; x < itemWidgets.Num(); x++)
				if (itemWidgets[x]->GetPrimaryGun() == primaries[i].GetPrimaryGunType())
				{
					itemWidgets[x]->AddToAmount(1);
					bCreateNewItem = false;
					break;
				}

			if (!bCreateNewItem)
				continue;

			auto newPrimary = CreateListItem();
			newPrimary->SetPrimaryGun(primaries[i].GetPrimaryGunType());
			newPrimary->AddToAmount(1);
			newPrimary->SetItemId(primaries[i].GetPrimaryId());
			newPrimary->AddPrimaryDelegate();

			itemWidgets.Emplace(newPrimary);
			ScrollBox->AddChild(newPrimary);
		}
	}
}

void USelectEquipmentListWidget::OpenListForTools(FGuid memberId, int toolIndex)
{
	SetVisibility(ESlateVisibility::Visible);

	CurrentMemberId = memberId;
	CurrentToolIndex = toolIndex;

	auto instance = USvUtilities::GetGameInstance(GetWorld());

	auto gameDataManager = instance->GetCurrentGameDataManager();
	auto gameData = gameDataManager->GetCurrentGameData();

	auto scrollBox = (UScrollBox *)GetWidgetFromName("ScrollBox");
	scrollBox->ClearChildren();

	TArray<USelectEquipmentListItemWidget *> itemWidgets;

	if (scrollBox)
	{
		auto emptyListItem = CreateListItem();
		emptyListItem->AddToolDelegate();
		scrollBox->AddChild(emptyListItem);
		// setup button on click function
		auto tools = gameData->GetAllTools();

		for (int i = 0; i < tools.Num(); i++)
		{
			if (tools[i].GetCrewMemberId() == FGuid::FGuid())
			{

				bool bCreateNewItem = true;

				for (int x = 0; x < itemWidgets.Num(); x++)
					if (tools[i].GetToolType() == itemWidgets[x]->GetToolType() && tools[i].GetTool() == itemWidgets[x]->GetTool())
					{
						itemWidgets[x]->AddToAmount(1);
						bCreateNewItem = false;
						break;
					}

				if (!bCreateNewItem)
					continue;

				auto newPrimary = CreateListItem();
				newPrimary->AddToolDelegate();
				newPrimary->SetToolAndType(tools[i].GetToolType(), tools[i].GetTool());
				newPrimary->AddToAmount(1);

				itemWidgets.Emplace(newPrimary);
				scrollBox->AddChild(newPrimary);
			}
		}
	}
}

void USelectEquipmentListWidget::PrimaryButtonClicked(EGun gunType, FGuid primaryId)
{
	auto instance = USvUtilities::GetGameInstance(GetWorld());

	auto gameDataManager = instance->GetCurrentGameDataManager();
	auto gameData = gameDataManager->GetCurrentGameData();

	auto currentPrimary = gameData->GetCrewPrimary(CurrentMemberId);

	gameData->UnnassignPrimaryFromCrew(currentPrimary->GetPrimaryId());
	if (gunType != EGun::INVALID)
		gameData->AssignPrimaryToCrew(primaryId, CurrentMemberId);

	ResetDetailsGrid();
	SetVisibility(ESlateVisibility::Hidden);
}

void USelectEquipmentListWidget::ToolButtonClicked(EToolType toolType, uint8 tool)
{
	auto instance = USvUtilities::GetGameInstance(GetWorld());

	auto gameDataManager = instance->GetCurrentGameDataManager();
	auto gameData = gameDataManager->GetCurrentGameData();

	auto tools = gameData->GetCrewMemberTools(CurrentMemberId);
	// current tool index is greater than tools when adding a new tool

	if (tools.Num() > CurrentToolIndex)
	{
		auto thisTool = tools[CurrentToolIndex];
		gameData->UnnassignToolFromCrew(thisTool->GetToolId());
	}

	if (toolType != EToolType::INVALID)
	{
		auto selectedTool = gameData->GetFirstUnequippedTool(toolType, tool);
		gameData->AssignToolToCrew(selectedTool->GetToolId(), CurrentMemberId);
	}

	ResetDetailsGrid();
	SetVisibility(ESlateVisibility::Hidden);
}

void USelectEquipmentListWidget::ResetDetailsGrid()
{
	auto crewWidget = (UUserWidget *)GetOuter()->GetOuter();
	auto crewDetails = (UCrewDetailsGridWidget *)crewWidget->GetWidgetFromName("CrewDetailsGrid");
	if (crewDetails)
		crewDetails->InitialiseGridForCrewMember(CurrentMemberId);
}

USelectEquipmentListItemWidget *USelectEquipmentListWidget::CreateListItem()
{
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/World/Components/Pages/Squad/SelectEquipmentListItem_Bp.SelectEquipmentListItem_Bp_C'"));
	if (UClass *hudUIWidgetClass = hudUIRef.TryLoadClass<USelectEquipmentListItemWidget>())
	{
		return CreateWidget<USelectEquipmentListItemWidget>(this, hudUIWidgetClass);
	}
	return nullptr;
}