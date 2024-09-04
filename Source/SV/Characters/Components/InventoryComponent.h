// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Data/Game/Resource/FResourceData.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	void AddToHeldResource(EResourceType rType, int amount);
	TMap<EResourceType, int> GetHeldResources();

	void AddToIntel();
	int GetIntel();
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TMap<EResourceType, int> HeldResource;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Intel;
};
