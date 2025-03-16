
// Fill out your copyright notice in the Description page of Project Settings.
#include "PostMovementCrouch.h"
#include "../../Environment/Components/VaultableComponent.h"
#include "../../Environment/Components/SkippableComponent.h"
#include "../../Characters/Components/GridMovementComponent.h"


UPostMovementCrouch::UPostMovementCrouch()
{
}

void UPostMovementCrouch::Setup(AActor *movedActor)
{
    MovedActor = movedActor;
}
void UPostMovementCrouch::BeginComponent()
{
    auto gridLocation = UGridUtilities::GetNormalisedGridLocation(MovedActor->GetActorLocation());
    gridLocation.Z -= 50;
    TArray<FVector> adjacentLocations;
    USvUtilities::GetAdjacentGridTiles(gridLocation, adjacentLocations);

    for (int i = 0; i < adjacentLocations.Num(); i++)
    {
        FHitResult Hit;
        GetWorld()->LineTraceSingleByChannel(Hit, gridLocation, adjacentLocations[i], USvUtilities::GetEnvironmentChannel());
        // DrawDebugLine(GetWorld(), gridLocation, adjacentLocations[i], FColor::Red, true, 60, 0, 5);

        if (Hit.GetActor())
        {
            UDebugMessages::LogDisplay(this, "found actor " + Hit.GetActor()->GetName() + " comp " + Hit.GetComponent()->GetName());

            if (Hit.GetActor()->GetComponentByClass<UVaultableComponent>() ||
                Hit.GetActor()->GetComponentByClass<USkippableComponent>())
            {
                auto movementComponent = MovedActor->GetComponentByClass<UGridMovementComponent>();

                if (movementComponent)
                    movementComponent->PostMovementCrouch();
            }
        }
    }
}

void UPostMovementCrouch::Finish()
{
    ClearInternalFlags(EInternalObjectFlags::Async);
}