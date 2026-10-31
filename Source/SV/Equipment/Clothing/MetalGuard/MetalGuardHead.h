// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/ClothingHelmet.h"
#include "MetalGuardHead.generated.h"

class UStaticMeshComponent;
class UClothingHitBoxComponent;

/**
 *
 */
UCLASS()
class SV_API AMetalGuardHead : public AClothingHelmet
{
	GENERATED_BODY()

public:
	AMetalGuardHead(const FObjectInitializer &ObjectInitializer);


protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *HeadMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClothingHitBoxComponent* HelmetHitBoxComponent;
};
