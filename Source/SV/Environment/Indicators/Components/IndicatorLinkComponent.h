// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IndicatorLinkComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UIndicatorLinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIndicatorLinkComponent();

	void SetPickupIndicatingTo(AActor* actor);
	AActor* GetPickupIndicatingTo();

	void SetToolIndicatingTo(AActor* actor);
	AActor* GetToolIndicatingTo();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) AActor* PickupIndicatingTo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) AActor* ToolIndicatingTo;
};
