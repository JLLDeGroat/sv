// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraOverlapComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Interfaces/SvChar.h"
#include "Components/CapsuleComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UCameraOverlapComponent::UCameraOverlapComponent(const FObjectInitializer& ObjectInitializer) : USphereComponent() {

	SetGenerateOverlapEvents(true);
	OnComponentBeginOverlap.AddDynamic(this, &UCameraOverlapComponent::Overlapped);
	OnComponentEndOverlap.AddDynamic(this, &UCameraOverlapComponent::OverlapEnded);

	SetCollisionResponseToAllChannels(ECR_Overlap);
}

void UCameraOverlapComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (GetOwner() == OtherActor) return;

	if (OtherActor->Implements<USvChar>() && 
		OtherComp->IsA<UCapsuleComponent>()) 
	{
		TScriptInterface<ISvChar> character = OtherActor;
		character->UpdateActorVisibility(false);
	}
}

void UCameraOverlapComponent::OverlapEnded(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	if (OtherActor->Implements<USvChar>() &&
		OtherComp->IsA<UCapsuleComponent>()) 
	{
		TScriptInterface<ISvChar> character = OtherActor;
		character->UpdateActorVisibility(true);
	}
}