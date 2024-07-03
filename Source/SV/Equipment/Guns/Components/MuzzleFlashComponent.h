// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "MuzzleFlashComponent.generated.h"

/**
 *
 */
UCLASS()
class SV_API UMuzzleFlashComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UMuzzleFlashComponent();
	void ActivateMuzzleFlash();
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UNiagaraComponent* FlashHeatPelletComponent;
};
