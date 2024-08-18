// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../Enums/EEquipmentEnums.h"
#include "PickupDetailsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UPickupDetailsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupDetailsComponent();

	void SetResourceType(EResourceType rType);
	void SetAmount(int amount);

	int GetAmount();
	EResourceType GetResourceType();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) EResourceType ResourceType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Amount;
};
