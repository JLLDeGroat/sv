// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Enums/ECharacterEnums.h"
#include "Selectable.generated.h"

class IMovable;
/**
 * 
 */
UINTERFACE(BlueprintType)
class SV_API USelectable : public UInterface
{
	GENERATED_BODY()
	
};

class SV_API ISelectable {

	GENERATED_BODY()

public:

	virtual FVector GetSelectableGridLocation() = 0;
	virtual bool TryGetAsMoveable(TScriptInterface<IMovable>& Moveable) = 0;

	virtual void TryVisualiseTargets() = 0;
	virtual bool IsControlType(ECharacterControl characterControl) = 0;
	virtual AActor* GetAsActor() = 0;
};
