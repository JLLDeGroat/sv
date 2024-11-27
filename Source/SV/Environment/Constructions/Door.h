// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "Door.generated.h"

class UToggleRadiusComponent;
class UToggleMovementComponent;
class UEnvironmentDetailsComponent;
/**
 * 
 */
UCLASS()
class SV_API ADoor : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	ADoor(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* DoorFrameComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* DoorComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UToggleMovementComponent* ToggleMovement;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UToggleRadiusComponent* ToggleRadius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UEnvironmentDetailsComponent* DetailsComponent;
	
};
