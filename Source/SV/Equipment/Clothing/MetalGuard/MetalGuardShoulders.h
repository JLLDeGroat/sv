// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/ClothingShoulders.h"
#include "MetalGuardShoulders.generated.h"

class UStaticMeshComponent;
class UClothingHitBoxComponent;
/**
 *
 */
UCLASS()
class SV_API AMetalGuardShoulders : public AClothingShoulders
{
	GENERATED_BODY()

public:
	AMetalGuardShoulders(const FObjectInitializer &ObjectInitializer);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *LeftShoulderMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *RightShoulderMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *RightShoulderHitBoxComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *LeftShoulderHitBoxComponent;
};
