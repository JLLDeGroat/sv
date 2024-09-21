// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Equipment.h"
#include "UrfGun.generated.h"

class UStaticMeshComponent;
class UGunFireComponent;
class UMuzzleFlashComponent;
class ULightAttachmentComponent;

UCLASS()
class SV_API AUrfGun : public AEquipment
{
	GENERATED_BODY()

public:

	AUrfGun(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;

protected:


	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MeshComponent1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MeshComponent2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MeshComponent3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MeshComponent4;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MeshComponent5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UGunFireComponent* GunFireComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UMuzzleFlashComponent* MuzzleFlashComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) ULightAttachmentComponent* LightAttachmentComponent;

};
