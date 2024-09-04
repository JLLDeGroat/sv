// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "PickupMeshComponent.generated.h"

/**
 *
 */
UCLASS()
class SV_API UPickupMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	UPickupMeshComponent();

	void SetMesh(FString mesh);

protected:

	UPROPERTY() FString MeshRef;

};
