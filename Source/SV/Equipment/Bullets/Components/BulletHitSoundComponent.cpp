// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletHitSoundComponent.h"
#include "../../../Characters/Components/AIComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/CapsuleComponent.h"

UBulletHitSoundComponent::UBulletHitSoundComponent(const FObjectInitializer& ObjectInitializer)
	: USphereComponent(ObjectInitializer) {
	SetGenerateOverlapEvents(true);
	OnComponentBeginOverlap.AddDynamic(this, &UBulletHitSoundComponent::Overlapped);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UBulletHitSoundComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	auto aiComp = OtherActor->GetComponentByClass<UAIComponent>();

	if (aiComp && OtherComp->IsA<UCapsuleComponent>()) {
		UDebugMessages::LogDisplay(this, "Activating AI on bulletSound " + OtherActor->GetName());
		aiComp->SetIsAiActive(true);
	}
}