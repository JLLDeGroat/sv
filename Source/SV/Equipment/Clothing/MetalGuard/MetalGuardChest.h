// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/ClothingChest.h"
#include "MetalGuardChest.generated.h"

class UStaticMeshComponent;
class UClothingHitBoxComponent;
/**
 *
 */
UCLASS()
class SV_API AMetalGuardChest : public AClothingChest
{
	GENERATED_BODY()

public:
	AMetalGuardChest(const FObjectInitializer &ObjectInitializer);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *FrontChestMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *BackChestMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *FrontChestHitBoxComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *BackChestHitBoxComponent;
};
