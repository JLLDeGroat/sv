// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "MuzzleFlashComponent.generated.h"

/**
 *
 */
UCLASS()
class SV_API UMuzzleFlashComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	UMuzzleFlashComponent();
	void ActivateMuzzleFlash();

	void SetFlashPelletColour(FLinearColor colour);
	void SetMuzzleFlashColour(FLinearColor colour);

	void SetMuzzleFlashSystem(FString sys);

	UNiagaraComponent* GetFlashHeatPelletComponent();
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UNiagaraComponent* FlashHeatPelletComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UNiagaraComponent* MuzzleFlashComponent;
};
