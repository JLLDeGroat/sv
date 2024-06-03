// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "../../Enums/EEquipmentEnums.h"
#include "GrenadeActionComponent.generated.h"

class AGrenadeIndicatorActor;
/**
 * 
 */
UCLASS()
class SV_API UGrenadeActionComponent : public UBaseActionComponent
{
	GENERATED_BODY()

public:

	UGrenadeActionComponent(const FObjectInitializer& ObjectInitializer);

	void DoAction();

	void SetGrenadeIndicatorActor(AGrenadeIndicatorActor* actor);

	EThrowable GetThrowableChosen();

protected:

	UPROPERTY() AGrenadeIndicatorActor* GrenadeIndicatorActor;

	UPROPERTY() EThrowable ThrowableChosen;

};
