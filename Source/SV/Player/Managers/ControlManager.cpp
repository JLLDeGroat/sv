
// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Interfaces/Selectable.h"
#include "../../Interfaces/Movable.h"
#include "../../Delegates/GameplayDelegates.h"
#include "../../Characters/Components/GridMovementComponent.h"
#include "SelectionManager.h"
#include "../Utility/GrenadeIndicatorActor.h"
#include "../Utility/GridSelectionActor.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Utilities/GridUtilities.h"
#include "../GamePlayerController.h"
#include "../Components/PawnCameraComponent.h"
#include "../../Delegates/HudDelegates.h"

UControlManager::UControlManager(const FObjectInitializer& ObjectInitializer) : UBaseControllerManager(ObjectInitializer) {
	bCanMouseDesignateSelectionDecal = true;
	bCanMouseDesignateExplosionRadiusActor = false;
}

void UControlManager::BeginPlay() {
	Super::BeginPlay();

	SelectionManager = GetOwner()->GetComponentByClass<USelectionManager>();

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud Delegates");

	hudDelegates->_OnHudItemHovered.AddDynamic(this, &UControlManager::OnUIItemHovered);
	hudDelegates->_OnHudItemUnhovered.AddDynamic(this, &UControlManager::OnUIItemUnhovered);

	auto gameplayDelegates = UGameplayDelegates::GetInstance();
	if (!gameplayDelegates)
		return UDebugMessages::LogError(this, "failed to get gameplay delegates");

	gameplayDelegates->_ChangeControlLimits.AddDynamic(this, &UControlManager::UpdateControlLimit);
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

	if (!val) GrenadeIndicatorActor->SetActorLocation(FVector(0, 0, -1000));
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
		//TODO: remove 50 to get to feet of soldier
		startLocation.Z -= 50;

		auto gridSteps = movableComponent->FindRoute(startLocation, localisedLocation);
		SelectionManager->SetLocationPath(gridSteps);
		if (gridSteps.Num() > 1) {
			for (int i = 1; i < gridSteps.Num(); i++) {
				DrawDebugLine(GetOwner()->GetWorld(), gridSteps[i - 1], gridSteps[i], FColor::Red, false, 1.0f, 0, 1.0f);
			}
		}
	}
}

void UControlManager::OnUIItemHovered() {
	UDebugMessages::LogDisplay(this, "hovered");

	if (bIsHovered) return;
	bIsHovered = true;

	bDisabledMouseDesignationOnHover = bCanMouseDesignateSelectionDecal;
	bDisabledMouseExplosionDesignationOnHover = bCanMouseDesignateExplosionRadiusActor;

	if (bDisabledMouseDesignationOnHover) SetCanMouseDesignateSelectionDecal(false);
	if (bDisabledMouseExplosionDesignationOnHover) SetCanMouseDesignateExplosionRadiusActor(false);
}
void UControlManager::OnUIItemUnhovered() {

	if (!bIsHovered) return;
	bIsHovered = false;

	UDebugMessages::LogDisplay(this, "unhovered");

	SetCanMouseDesignateSelectionDecal(bDisabledMouseDesignationOnHover);
	SetCanMouseDesignateExplosionRadiusActor(bDisabledMouseExplosionDesignationOnHover);
}

void UControlManager::UpdateControlLimit(EControlLimit controlLimit) {
	if (controlLimit == EControlLimit::CL_NONE) {
		auto selectionManager = GetOwner()->GetComponentByClass<USelectionManager>();
		if (!selectionManager)
			return UDebugMessages::LogError(this, "failed to get selection manager");

		//forces a re-tick
		selectionManager->SetCurrentMousedLocation(FVector(-100, 0, 0));
	}

	ControlLimits = controlLimit;
}
EControlLimit UControlManager::GetControlLimit() {
	return ControlLimits;
}