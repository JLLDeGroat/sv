// Fill out your copyright notice in the Description page of Project Settings.

#include "SkippableComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Characters/Components/SkipObstacleComponent.h"
// Sets default values for this component's properties
USkippableComponent::USkippableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetGenerateOverlapEvents(true);
	SetCollisionObjectType(USvUtilities::GetTriggerableChannel());

	OnComponentBeginOverlap.AddDynamic(this, &USkippableComponent::Overlapped);
}

// Called when the game starts
void USkippableComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USkippableComponent::Overlapped(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									 int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

	auto detailsComp = OtherActor->GetComponentByClass<UCharacterDetailsComponent>();
	if (OtherComp->IsA<UCapsuleComponent>() && detailsComp && detailsComp->GetCanVault())
	{
		auto skippingComponent = OtherActor->GetComponentByClass<USkipObstacleComponent>();

		if (skippingComponent)
			skippingComponent->ActivateSkip();
	}
}