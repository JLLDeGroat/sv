// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Enums/ECharacterEnums.h"
#include "SvChar.generated.h"

class IHitComponent;
class ABaseCharacter;
/**
 * 
 */
UINTERFACE()
class SV_API USvChar : public UInterface
{
	GENERATED_BODY()
	
};

class SV_API ISvChar {

	GENERATED_BODY()

public:

	virtual FVector GetSelectableGridLocation() = 0;
	virtual bool IsControlType(ECharacterControl characterControl) = 0;
	virtual TArray<TScriptInterface<IHitComponent>> GetHitComponents() = 0;

	virtual AActor* GetAsActor() = 0;

	virtual void UpdateActorVisibility(bool value) = 0;

};
