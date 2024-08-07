// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletSoundComponent.h"
#include "../../../Characters/Components/AIComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/CapsuleComponent.h"

UBulletSoundComponent::UBulletSoundComponent(const FObjectInitializer& ObjectInitializer)
	: USphereComponent(ObjectInitializer) {
	SetGenerateOverlapEvents(true);
	OnComponentBeginOverlap.AddDynamic(this, &UBulletSoundComponent::Overlapped);

}

void UBulletSoundComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	auto aiComp = OtherActor->GetComponentByClass<UAIComponent>();

	if (aiComp && OtherComp->IsA<UCapsuleComponent>()) {
		UDebugMessages::LogDisplay(this, "Activating AI on bulletSound " + OtherActor->GetName());
		aiComp->SetIsAiActive(true);
	}
}