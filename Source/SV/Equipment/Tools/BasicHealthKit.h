// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Equipment.h"
#include "BasicHealthKit.generated.h"

class UStaticMeshComponent;
class UNiagaraComponent;

UCLASS()
class SV_API ABasicHealthKit : public AEquipment
{
	GENERATED_BODY()

public:

	ABasicHealthKit(const FObjectInitializer& ObjectInitializer);


	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent* RootSceneComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* HealthKitMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UNiagaraComponent* SprayEffectsComponent;

};
