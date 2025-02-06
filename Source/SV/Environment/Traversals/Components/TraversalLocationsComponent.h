// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraversalLocationsComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UTraversalLocationsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTraversalLocationsComponent();

public:

	void SetStartGridLocation(FVector start);
	void SetEndGridLocation(FVector end);

	FVector GetStartGridLocation();
	FVector GetEndGridLocation();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector StartLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector EndLocation;

};
