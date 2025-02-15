// Fill out your copyright notice in the Description page of Project Settings.

#include "CrewDetailsGridStatWidget.h"
#include "../../../../../../Utilities/SvUtilities.h"
#include "../../../../../../Instance/SvGameInstance.h"
#include "../../../../../../Instance/Managers/CurrentGameDataManager.h"
#include "../../../../../Helpers/UserWidgetHelpers.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "CDGDDItemWidget.h"
#include "CDGSItemWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UCrewDetailsGridStatWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StatTitle)
        UUserWidgetHelpers::DesignText(StatTitle, 32);

    if (DDTitle)
        UUserWidgetHelpers::DesignText(DDTitle, 28);

    ClearGrids();
}

void UCrewDetailsGridStatWidget::SetDetailsForCrew(TArray<TSubclassOf<UBaseDD>> dAndDs, TMap<FString, int> stats)
{
    ClearGrids();

    for (TSubclassOf<UBaseDD> dAndD : dAndDs)
    {
        UBaseDD *baseDD = NewObject<UBaseDD>(this, dAndD);
        auto ddItem = CreateDDItem();
        if (ddItem && baseDD)
        {
            ddItem->SetDescription(baseDD->GetDeviationOrDirectiveDescription());
            if (baseDD->GetDeviationOrDirective() == EDDType::DD_Directive)
                ddItem->SetImageAsDirective();
            else
                ddItem->SetImageAsDeviation();

            ddItem->SetTitle(baseDD->GetDeviationOrDirectiveTitle());

            DDBox->AddChildToVerticalBox(ddItem);
        }

        baseDD = nullptr;
    }

    for (TPair<FString, int> stat : stats)
    {
        auto statItem = CreateStatItem();
        if (statItem)
        {
            statItem->SetTitle(stat.Key);
            statItem->SetValue(stat.Value);

            StatBox->AddChildToVerticalBox(statItem);
        }
    }
}

UCDGSItemWidget *UCrewDetailsGridStatWidget::CreateStatItem()
{
    FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/World/Components/Pages/Squad/SubComponents/CDGSItemWidget_Bp.CDGSItemWidget_Bp_C'"));
    if (UClass *hudUIWidgetClass = hudUIRef.TryLoadClass<UCDGSItemWidget>())
    {
        return CreateWidget<UCDGSItemWidget>(this, hudUIWidgetClass);
    }
    return nullptr;
}
UCDGDDItemWidget *UCrewDetailsGridStatWidget::CreateDDItem()
{
    FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/World/Components/Pages/Squad/SubComponents/CDGDDItemWidget_Bp.CDGDDItemWidget_Bp_C'"));
    if (UClass *hudUIWidgetClass = hudUIRef.TryLoadClass<UCDGDDItemWidget>())
    {
        return CreateWidget<UCDGDDItemWidget>(this, hudUIWidgetClass);
    }
    return nullptr;
}

void UCrewDetailsGridStatWidget::ClearGrids()
{
    if (StatBox)
        StatBox->ClearChildren();

    if (DDBox)
        DDBox->ClearChildren();
}