// Fill out your copyright notice in the Description page of Project Settings.


#include "LightAttachmentComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "Components/SpotLightComponent.h"


ULightAttachmentComponent::ULightAttachmentComponent() {
	auto meshRef = "/Script/Engine.StaticMesh'/Game/Equipment/TorchAttachment.TorchAttachment'";
	auto staticMesh = USvUtilities::GetStaticMesh(meshRef);
	if (staticMesh) {
		SetStaticMesh(staticMesh);
	}
	DirectionalLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	DirectionalLightComponent->SetupAttachment(GetAttachmentRoot(), FName("LightSocket"));

	DirectionalLightComponent->SetIntensity(20000);
	DirectionalLightComponent->SetAttenuationRadius(1000);
	DirectionalLightComponent->SetInnerConeAngle(15);
	DirectionalLightComponent->SetOuterConeAngle(40);
	DirectionalLightComponent->SetSoftSourceRadius(100);
	DirectionalLightComponent->SetRelativeRotation(FRotator(00, 180, 0));
}