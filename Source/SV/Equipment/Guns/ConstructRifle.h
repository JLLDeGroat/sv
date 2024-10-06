// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Equipment.h"
#include "ConstructRifle.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UGunFireComponent;
class UMuzzleFlashComponent;
class UGunActivationComponent;

UCLASS()
class SV_API AConstructRifle : public AEquipment
{
	GENERATED_BODY()

public:

	AConstructRifle(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent* RootSceneComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* BackSectionComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* CannonSectionComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MidSectionComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MidSection2Component;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MidSection3Component;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MidSection4Component;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* BackConnectorComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* FrontConnectorComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UGunFireComponent* GunFireComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UMuzzleFlashComponent* MuzzleFlashComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UGunActivationComponent* ActivationComponent;
};
