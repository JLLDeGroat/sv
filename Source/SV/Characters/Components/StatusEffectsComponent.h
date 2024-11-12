// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Enums/ECharacterEnums.h"
#include "StatusEffectsComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UStatusEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusEffectsComponent();

	void AddToDebuff(EDebuffType debuffType, float amount);
	void AddToDebuff(TMap<EDebuffType, float> map);
	bool TryGetDebuffValue(EDebuffType debuffType, float& amount);

protected:

	UPROPERTY() TMap<EDebuffType, float> DebuffMap;
};
