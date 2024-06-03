// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionManager.h"
#include "../../Interfaces/Selectable.h"
#include "../../Interfaces/Movable.h"
#include "../../Characters/Components/GridMovementComponent.h"

bool USelectionManager::TrySetSelected(AActor* actor) {
	ISelectable* selectable = Cast<ISelectable>(actor);
	if (selectable) {
		Selected.SetInterface(selectable);
		Selected.SetObject(actor);

		return true;
	}
	else {
		Selected.SetInterface(NULL);
		Selected.SetObject(NULL);
	}
	return false;
}
TScriptInterface<ISelectable> USelectionManager::GetSelected() {
	return Selected;
}

void USelectionManager::SetLocationPath(TArray<FVector> vectors) {
	SelectedLocationPath = vectors;
}
TArray<FVector> USelectionManager::GetLocationPath() {
	return SelectedLocationPath;
}

void USelectionManager::SetCurrentMousedLocation(FVector mousedLocation) {
	MousedLocation = mousedLocation;
}
FVector USelectionManager::GetCurrentMousedLocation() {
	return MousedLocation;
}
