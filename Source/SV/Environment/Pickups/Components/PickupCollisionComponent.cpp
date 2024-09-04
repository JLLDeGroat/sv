// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupCollisionComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../../Characters/Components/PickupResourceComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UPickupCollisionComponent::UPickupCollisionComponent() {
	OnComponentBeginOverlap.AddDynamic(this, &UPickupCollisionComponent::Overlapped);
	OnComponentEndOverlap.AddDynamic(this, &UPickupCollisionComponent::EndOverlap);
	SetGenerateOverlapEvents(true);
}

void UPickupCollisionComponent::BeginPlay() {
	Super::BeginPlay();

	SetBoxExtent(FVector(200));
}

void UPickupCollisionComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherComp->IsA<UCapsuleComponent>()) {
		auto pickupComponent = OtherActor->GetComponentByClass<UPickupResourceComponent>();

		if (!pickupComponent)
			return UDebugMessages::LogError(this, OtherActor->GetName() + " does not have pickup component");


		pickupComponent->AddToPickupActors(GetOwner());
	}
}

void UPickupCollisionComponent::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherComp->IsA<UCapsuleComponent>()) {
		auto pickupComponent = OtherActor->GetComponentByClass<UPickupResourceComponent>();
		if (pickupComponent) {
			pickupComponent->RemovePickupActor(GetOwner());
		}
	}
}