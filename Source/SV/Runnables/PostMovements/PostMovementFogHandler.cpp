
// Fill out your copyright notice in the Description page of Project Settings.
#include "PostMovementFogHandler.h"
#include "../../Characters/Components/FogHandlerComponent.h"
#include "../../Characters/Components/HealthAndStatusWidgetComponent.h"
#include "../../Environment/Fog/Components/FogSectionComponent.h"
#include "../../Environment/Fog/FogManager.h"


UPostMovementFogHandler::UPostMovementFogHandler()
{
}

void UPostMovementFogHandler::Setup(AActor *movedActor)
{
    MovedActor = movedActor;
}
#pragma optimize("", off)
void UPostMovementFogHandler::BeginComponent()
{
    auto fogComponent = MovedActor->GetComponentByClass<UFogHandlerComponent>();
    if (fogComponent && !fogComponent->GetIsAi())
        RemoveFogAroundCharacter();
    else
    {
        AFogManager *fogManager = nullptr;
        auto &actors = GetWorld()->GetCurrentLevel()->Actors;
        for (int i = 0; i < actors.Num(); i++)
        {
            if (actors[i])
            {
                if (AFogManager *fManager = Cast<AFogManager>(actors[i]))
                {
                    fogManager = fManager;
                    break;
                }
            }
        }

        if (!fogManager)
        {
            UDebugMessages::LogError(this, "failed to get fog manager, PostMovement fog manager cannot hide enemy details if inside fog");
            return;
        }
        auto allFogSections = fogManager->GetFogSections();
        for (UFogSectionComponent *fogComp : allFogSections)
        {
            auto movedActorLoc = MovedActor->GetActorLocation();
            auto compLoc = fogComp->GetComponentLocation();

            if (FVector::Dist(movedActorLoc, compLoc) < 100)
            {
                auto statusComponent = MovedActor->GetComponentByClass<UHealthAndStatusWidgetComponent>();

                if (!statusComponent)
                {
                    UDebugMessages::LogError(this, "not hiding or showing status component, status component was null");
                    return;
                }
                UDebugMessages::LogDisplay(this, "found comp " + fogComp->GetName() + " at loc: " + compLoc.ToString());
                if (fogComp->GetIsInFog())
                {
                    FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
                        [statusComponent]
                        {
                            statusComponent->HideComponent();
                        },
                        TStatId(),
                        nullptr, ENamedThreads::GameThread);
                }
                else
                {
                    FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
                        [statusComponent]
                        {
                            statusComponent->ShowComponent();
                        },
                        TStatId(),
                        nullptr, ENamedThreads::GameThread);
                }
            }
        }
    }
}

void UPostMovementFogHandler::Finish()
{
    ClearInternalFlags(EInternalObjectFlags::Async);
}

void UPostMovementFogHandler::RemoveFogAroundCharacter()
{
    auto fogComponent = MovedActor->GetComponentByClass<UFogHandlerComponent>();
    TArray<FVector> validAdjacentTiles;
    USvUtilities::GetAdjacentTilesForFogCalculation(MovedActor, validAdjacentTiles);
    validAdjacentTiles.Emplace(MovedActor->GetActorLocation());

    auto fogSections = fogComponent->GetFogSectionComponents();

    for (int i = 0; i < fogSections.Num(); i++)
    {
        auto fogSectionToChange = fogSections[i];
        TArray<FVector> locationsToCheckFrom;

        for (int x = 0; x < validAdjacentTiles.Num(); x++)
            if (FVector::Dist(fogSectionToChange->GetComponentLocation(), validAdjacentTiles[x]) <= fogComponent->GetScaledSphereRadius())
                locationsToCheckFrom.Emplace(validAdjacentTiles[x]);

        for (int j = 0; j < locationsToCheckFrom.Num(); j++)
        {
            if (fogComponent->ShouldRemoveFog(fogSectionToChange, locationsToCheckFrom[j]))
            {
                FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
                    [fogSectionToChange]
                    {
                        fogSectionToChange->SetAsNoFog();
                    },
                    TStatId(), nullptr, ENamedThreads::GameThread);
                break;
            }
        }
    }
}
#pragma optimize("", on)