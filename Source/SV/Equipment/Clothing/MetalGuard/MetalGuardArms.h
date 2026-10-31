// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/ClothingArms.h"
#include "MetalGuardArms.generated.h"

class UStaticMeshComponent;
class UClothingHitBoxComponent;
/**
 *
 */
UCLASS()
class SV_API AMetalGuardArms : public AClothingArms
{
	GENERATED_BODY()

public:
	AMetalGuardArms(const FObjectInitializer &ObjectInitializer);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *LeftUpperArmMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *RightUpperArmMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *LeftLowerArmMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *RightLowerArmMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *LeftUpperArmHitBoxComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *RightUpperArmHitBoxComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *LeftLowerArmHitBoxComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent *RightLowerArmHitBoxComponent;
};
