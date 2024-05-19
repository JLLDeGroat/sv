// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseControllerManager.h"
#include "SelectionManager.generated.h"

class ISelectable;

/**
 *
 */
UCLASS()
class SV_API USelectionManager : public UBaseControllerManager
{
	GENERATED_BODY()

public:

	bool TrySetSelected(AActor* actor);
	TScriptInterface<ISelectable> GetSelected();

	void SetLocationPath(TArray<FVector> vectors);
	TArray<FVector> GetLocationPath();

	void SetCurrentMousedLocation(FVector mousedLocation);
	FVector GetCurrentMousedLocation();

private:

	UPROPERTY() TScriptInterface<ISelectable> Selected;

	UPROPERTY() TArray<FVector> SelectedLocationPath;

	UPROPERTY() FVector MousedLocation;

};
