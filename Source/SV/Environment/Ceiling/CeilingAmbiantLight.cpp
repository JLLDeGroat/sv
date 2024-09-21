// Fill out your copyright notice in the Description page of Project Settings.


#include "CeilingAmbiantLight.h"
#include "Components/SpotLightComponent.h"

ACeilingAmbiantLight::ACeilingAmbiantLight(const FObjectInitializer& ObjectInitializer)
	:AEnvironmentActor(ObjectInitializer) {

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComponent;

	MainLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("MainLight"));
	BackLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("BackLight"));

	MainLightComponent->SetupAttachment(RootComponent);
	BackLightComponent->SetupAttachment(RootComponent);

	//BackLightComponent->SetRelativeLocation(FVector(0, 0, 100));

	MainLightComponent->SetIntensityUnits(ELightUnits::EV);
	MainLightComponent->SetIntensity(9.5f);
	MainLightComponent->SetLightColor(FLinearColor(.53f, .73f, 1, 1));
	MainLightComponent->SetInnerConeAngle(13);
	MainLightComponent->SetOuterConeAngle(32);
	MainLightComponent->SetAttenuationRadius(4000);

	BackLightComponent->SetIntensityUnits(ELightUnits::EV);
	BackLightComponent->SetIntensity(4.5f);
	BackLightComponent->SetLightColor(FLinearColor(.53f, .73f, 1, 1));
	BackLightComponent->SetInnerConeAngle(1);
	BackLightComponent->SetOuterConeAngle(80);
	BackLightComponent->SetAttenuationRadius(4000);
}

FLinearColor ACeilingAmbiantLight::ChooseColour() {
	auto random = FMath::RandRange(0, 1001);

	if (random > 750) return FLinearColor(.53f, .73f, 1, 1);
	else if (random > 500) return FLinearColor(.95f, .66f, 1, 1);
	else if (random > 200) return FLinearColor(.58f, .51f, 1, 1);
	else return FLinearColor(.64f, 1, .95f, 1);
}