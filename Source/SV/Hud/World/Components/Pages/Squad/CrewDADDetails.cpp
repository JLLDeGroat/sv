#include "CrewDADDetails.h"
#include "../../../../Helpers/UserWidgetHelpers.h"
#include "SubComponents/CDGDDItemWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "../../../SquadPageWidget.h"
#include "../../../../../Characters/DandD/Base/BaseDD.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/VerticalBox.h"

void UCrewDADDetails::NativeConstruct()
{
    Super::NativeConstruct();
    UUserWidgetHelpers::DesignButton(CloseButton);
    UUserWidgetHelpers::DesignBorder(BackBorder, "Script/Engine.Material'/Game/Materials/Widgets/LargeBack_M.LargeBack_M'");

    CloseButton->OnClicked.AddDynamic(this, &UCrewDADDetails::OnCloseClicked);

    SetVisibility(ESlateVisibility::Hidden);
}

UCDGDDItemWidget *UCrewDADDetails::CreateDDItem()
{
    FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/World/Components/Pages/Squad/SubComponents/CDGDDItemWidget_Bp.CDGDDItemWidget_Bp_C'"));
    if (UClass *hudUIWidgetClass = hudUIRef.TryLoadClass<UCDGDDItemWidget>())
    {
        return CreateWidget<UCDGDDItemWidget>(this, hudUIWidgetClass);
    }
    return nullptr;
}

void UCrewDADDetails::OnCloseClicked()
{
    SetVisibility(ESlateVisibility::Hidden);
}

void UCrewDADDetails::SetSquadPage(USquadPageWidget *page)
{
    SquadPage = page;
}

void UCrewDADDetails::SetupDADDetails(TArray<TSubclassOf<UBaseDD>> danDs)
{
    SetVisibility(ESlateVisibility::Visible);
    DirectiveBox->ClearChildren();
    DeviationBox->ClearChildren();

    for (TSubclassOf<UBaseDD> dAndD : danDs)
    {
        UDebugMessages::LogDisplay(this, "Adding DAD to details: " + dAndD->GetName());
        UBaseDD *baseDD = NewObject<UBaseDD>(this, dAndD);
        auto ddItem = CreateDDItem();
        if (ddItem && baseDD)
        {
            ddItem->SetDescription(baseDD->GetDeviationOrDirectiveDescription());
            if (baseDD->GetDeviationOrDirective() == EDDType::DD_Directive)
            {
                ddItem->SetImageAsDirective();
                DirectiveBox->AddChild(ddItem);
            }
            else
            {
                ddItem->SetImageAsDeviation();
                DeviationBox->AddChild(ddItem);
            }
            ddItem->SetTitle(baseDD->GetDeviationOrDirectiveTitle());
        }

        baseDD = nullptr;
    }
}