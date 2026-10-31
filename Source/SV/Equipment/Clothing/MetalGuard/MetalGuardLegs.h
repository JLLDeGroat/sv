// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/ClothingLegs.h"
#include "MetalGuardLegs.generated.h"

class UStaticMeshComponent;
class UClothingHitBoxComponent;
/**
 *
 */
UCLASS()
class SV_API AMetalGuardLegs : public AClothingLegs
{
	GENERATED_BODY()

public:
	AMetalGuardLegs(const FObjectInitializer &ObjectInitializer);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *LeftUpperLegMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *RightUpperLegMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *LeftLowerLegMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *RightLowerLegMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *LeftUpperLegHitBoxComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *RightUpperLegHitBoxComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *LeftLowerLegHitBoxComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *RightLowerLegHitBoxComponent;
};
