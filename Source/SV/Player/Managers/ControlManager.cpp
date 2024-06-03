
// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Interfaces/Selectable.h"
#include "../../Interfaces/Movable.h"
#include "../../Characters/Components/GridMovementComponent.h"
#include "SelectionManager.h"
#include "../Utility/GrenadeIndicatorActor.h"
#include "../Utility/GridSelectionActor.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Utilities/GridUtilities.h"
#include "../GamePlayerController.h"
#include "../Components/PawnCameraComponent.h"

UControlManager::UControlManager(const FObjectInitializer& ObjectInitializer) : UBaseControllerManager(ObjectInitializer) {
	bCanMouseDesignateSelectionDecal = true;
	bCanMouseDesignateExplosionRadiusActor = false;
}

void UControlManager::BeginPlay() {
	Super::BeginPlay();

	SelectionManager = GetOwner()->GetComponentByClass<USelectionManager>();
}

void UControlManager::SetGrenadeIndicatorActor(AGrenadeIndicatorActor* indicatorActor) {
	GrenadeIndicatorActor = indicatorActor;
}
FVector UControlManager::GetGrenadeIndicatorActorLocation() {
	return GrenadeIndicatorActor->GetActorLocation();
}

bool UControlManager::GetCanMouseDesignateSelectionDecal() {
	return bCanMouseDesignateSelectionDecal;
}
void UControlManager::SetCanMouseDesignateSelectionDecal(bool val) {
	bCanMouseDesignateSelectionDecal = val;
	GridSelectionActor->SetVisibility(val);
}
void UControlManager::SetGridSelectionActor(AGridSelectionActor* gridSelectionActor) {
	GridSelectionActor = gridSelectionActor;
}
AGridSelectionActor* UControlManager::GetGridSelectionActor() {
	return GridSelectionActor;
}

bool UControlManager::GetCanMouseDesignateExplosionRadiusActor() {
	return bCanMouseDesignateExplosionRadiusActor;
}
void UControlManager::SetCanMouseDesignateExplosionRadiusActor(bool val) {
	bCanMouseDesignateExplosionRadiusActor = val;
	GrenadeIndicatorActor->SetVisibility(val);
}

void UControlManager::TickShowGrenadeIndicator(FVector mouseLocation) {
	if (!GrenadeIndicatorActor)
		return UDebugMessages::LogError(this, "failed to get grenade indicator actor");

	auto selected = SelectionManager->GetSelected();
	auto selectedLocation = selected->GetAsActor()->GetActorLocation();

	if (FVector::Dist(mouseLocation, selectedLocation) > GrenadeIndicatorActor->GetRange()) {
		auto lookat = UGridUtilities::FindLookAtRotation(selectedLocation, mouseLocation);
		auto correctedLocation = selectedLocation + (lookat.Vector() * 600);
		GrenadeIndicatorActor->SetActorLocation(correctedLocation);
	}
	else GrenadeIndicatorActor->SetActorLocation(mouseLocation);
}

void UControlManager::TickFindMovementPath(FVector localisedLocation) {
	SelectionManager->SetCurrentMousedLocation(localisedLocation);
	auto selected = SelectionManager->GetSelected();
	TScriptInterface<IMovable> movable;
	if (selected && selected->TryGetAsMoveable(movable) &&
		selected->IsControlType(ECharacterControl::CC_Player))
	{
		auto movableComponent = movable->GetGridMovementComponent();
		auto startLocation = selected->GetSelectableGridLocation();

		auto gridSteps = movableComponent->FindRoute(startLocation, localisedLocation);
		SelectionManager->SetLocationPath(gridSteps);
		if (gridSteps.Num() > 1) {
			for (int i = 1; i < gridSteps.Num(); i++) {
				DrawDebugLine(GetOwner()->GetWorld(), gridSteps[i - 1], gridSteps[i], FColor::Red, false, 1.0f, 0, 1.0f);
			}
		}
	}
}