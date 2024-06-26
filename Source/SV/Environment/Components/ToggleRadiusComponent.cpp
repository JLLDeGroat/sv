// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleRadiusComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Characters/Components/ActivateTogglesComponent.h"
#include "ToggleMovementComponent.h"

// Sets default values for this component's properties
UToggleRadiusComponent::UToggleRadiusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetGenerateOverlapEvents(true);
	SetCollisionObjectType(USvUtilities::GetTriggerableChannel());

	OnComponentBeginOverlap.AddDynamic(this, &UToggleRadiusComponent::Overlapped);
	OnComponentEndOverlap.AddDynamic(this, &UToggleRadiusComponent::EndOverlap);
	// ...
}


// Called when the game starts
void UToggleRadiusComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UToggleRadiusComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	auto comp = OtherActor->GetComponentByClass<UActivateTogglesComponent>();
	auto toggleComp = GetOwner()->GetComponentByClass<UToggleMovementComponent>();
	if (comp && toggleComp) {
		comp->AddToggleComponent(toggleComp);
	}
	else return UDebugMessages::LogError(this, "failed to get other toggle component and this owners movement component");
}

void UToggleRadiusComponent::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	auto comp = OtherActor->GetComponentByClass<UActivateTogglesComponent>();
	auto toggleComp = GetOwner()->GetComponentByClass<UToggleMovementComponent>();

	if (comp && toggleComp) {
		comp->RemoveToggleComponent(toggleComp);
	}
	else return UDebugMessages::LogError(this, "failed to get other toggle component and this owners movement component");
}