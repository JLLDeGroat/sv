// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakeComponent.h"
#include "Camera/CameraShakeBase.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UCameraShakeComponent::UCameraShakeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	const FString Path = TEXT("/Game/CameraShake/GunFireCameraShake_Bp.GunFireCameraShake_Bp_C");
	//// Load the class
	if (GetOwner()) {
		UClass* gunFireCameraShakeClass = StaticLoadClass(UCameraShakeBase::StaticClass(), GetOwner(), *Path);
		if (gunFireCameraShakeClass)
			GunFireShakeComponent = gunFireCameraShakeClass;
	}
}

void UCameraShakeComponent::StartGunFireShake() {
	if (GunFireShakeComponent) {
		GetOwner<APlayerController>()->PlayerCameraManager->StartCameraShake(GunFireShakeComponent, 5, ECameraShakePlaySpace::CameraLocal, FRotator(1, 1, 1));
	}
	else
		UDebugMessages::LogError(this, "no shake component");
}

