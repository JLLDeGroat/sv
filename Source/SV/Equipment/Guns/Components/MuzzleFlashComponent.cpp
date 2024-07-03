// Fill out your copyright notice in the Description page of Project Settings.


#include "MuzzleFlashComponent.h"
#include "../../../Utilities/SvUtilities.h"

UMuzzleFlashComponent::UMuzzleFlashComponent() {
	auto basicAssetRef = "/Script/Niagara.NiagaraSystem'/Game/Effects/Trails/MuzzleFlash_N.MuzzleFlash_N'";
	auto niagaraSystem = USvUtilities::GetNiagaraSystem(basicAssetRef);
	if (niagaraSystem) {
		SetAsset(niagaraSystem);
	}

	auto heatPellet = "/Script/Niagara.NiagaraSystem'/Game/Effects/Trails/MuzzleFlashHeatPellet_N.MuzzleFlashHeatPellet_N'";
	auto heatPelletSystem = USvUtilities::GetNiagaraSystem(heatPellet);

	FlashHeatPelletComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlashHeatPellet"));
	FlashHeatPelletComponent->SetupAttachment(GetAttachmentRoot());
	
	if (heatPelletSystem)
		FlashHeatPelletComponent->SetAsset(heatPelletSystem);
}


void UMuzzleFlashComponent::ActivateMuzzleFlash() {
	auto currentRot = GetRelativeRotation();
	currentRot.Roll = FMath::RandRange(-180, 180);
	SetRelativeRotation(currentRot);
	
	Activate(true);
	FlashHeatPelletComponent->Activate(true);
}