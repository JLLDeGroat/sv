// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeviantDirectiveComponent.generated.h"

class UOnGunFireBase;
class OnBeginTargetingBase;
class UBaseDD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UDeviantDirectiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDeviantDirectiveComponent();

	TArray<UOnGunFireBase *> GetGunFireDDs();

	void AddDirectiveOrDeviant(TSubclassOf<UBaseDD> baseDD);

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<UOnGunFireBase *> GunFireComponents;

	UPROPERTY(BlueprintReadOnly)
	TArray<UBaseDD *> Components;
};
