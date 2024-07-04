// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Equipment.h"
#include "PeaRifle.generated.h"

class UStaticMeshComponent;
class UGunFireComponent;
class UMuzzleFlashComponent;
class ULightAttachmentComponent;
/**
 * 
 */
UCLASS()
class SV_API APeaRifle : public AEquipment
{
	GENERATED_BODY()

public:

	APeaRifle(const FObjectInitializer& ObjectInitializer);

	virtual void SetupAttachVector() override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* GunMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UGunFireComponent* GunFireComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UMuzzleFlashComponent* MuzzleFlashComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) ULightAttachmentComponent* LightAttachmentComponent;

	
};
