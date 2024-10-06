// Fill out your copyright notice in the Description page of Project Settings.


#include "MuzzleFlashComponent.h"
#include "../../../Utilities/SvUtilities.h"

UMuzzleFlashComponent::UMuzzleFlashComponent() {

	MuzzleFlashComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleFlashComp"));
	FlashHeatPelletComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlashHeatPellet"));

	FlashHeatPelletComponent->SetupAttachment(GetAttachmentRoot());
	MuzzleFlashComponent->SetupAttachment(GetAttachmentRoot());


	auto basicAssetRef = "/Script/Niagara.NiagaraSystem'/Game/Effects/Trails/MuzzleFlash_N.MuzzleFlash_N'";
	auto niagaraSystem = USvUtilities::GetNiagaraSystem(basicAssetRef);
	if (niagaraSystem) {
		MuzzleFlashComponent->SetAsset(niagaraSystem);
	}

	auto heatPellet = "/Script/Niagara.NiagaraSystem'/Game/Effects/Trails/MuzzleFlashHeatPellet_N.MuzzleFlashHeatPellet_N'";
	auto heatPelletSystem = USvUtilities::GetNiagaraSystem(heatPellet);
	if (heatPelletSystem)
		FlashHeatPelletComponent->SetAsset(heatPelletSystem);

}

void UMuzzleFlashComponent::SetMuzzleFlashSystem(FString sys) {
	auto niagaraSystem = USvUtilities::GetNiagaraSystem(sys);
	if (niagaraSystem) {
		MuzzleFlashComponent->SetAsset(niagaraSystem);
	}
}

void UMuzzleFlashComponent::ActivateMuzzleFlash() {
	/*auto currentRot = GetRelativeRotation();
	currentRot.Roll = FMath::RandRange(-180, 180);
	SetRelativeRotation(currentRot);*/

	MuzzleFlashComponent->Activate(true);
	FlashHeatPelletComponent->Activate(true);
}

UNiagaraComponent* UMuzzleFlashComponent::GetFlashHeatPelletComponent() {
	return FlashHeatPelletComponent;
}

void UMuzzleFlashComponent::SetFlashPelletColour(FLinearColor colour) {
	FlashHeatPelletComponent->SetVariableVec4(FName("FlashColour"), colour);
}

void UMuzzleFlashComponent::SetMuzzleFlashColour(FLinearColor colour) {
	MuzzleFlashComponent->SetVariableVec4(FName("MuzzleFlash"), colour);
}