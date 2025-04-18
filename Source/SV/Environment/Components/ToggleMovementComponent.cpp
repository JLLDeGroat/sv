// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "../../Delegates/GameplayDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Characters/Components/TargetingComponent.h"
#include "../../Characters/Components/ActionsComponent.h"

// Sets default values for this component's properties
UToggleMovementComponent::UToggleMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Location1 = FVector::ZeroVector;
	Location2 = FVector::ZeroVector;
	MovingToLocation = FVector::ZeroVector;
	MovingMesh = nullptr;
}


// Called when the game starts
void UToggleMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UToggleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto newLocation = UKismetMathLibrary::VInterpTo_Constant(MovingMesh->GetRelativeLocation(), MovingToLocation, DeltaTime, 200);
	MovingMesh->SetRelativeLocation(newLocation);

	if (FVector::Dist(MovingMesh->GetRelativeLocation(), MovingToLocation) < 1) {
		SetComponentTickEnabled(false);

		auto gamePlayDelegates = UGameplayDelegates::GetInstance();
		if (!gamePlayDelegates)
			return UDebugMessages::LogError(this, "failed to get gameplay delegates");

		gamePlayDelegates->_ChangeControlLimits.Broadcast(EControlLimit::CL_NONE);

		auto selected = USvUtilities::AttemptToGetCurrentSelectedActor(GetWorld());
		if (selected) {
			auto targeting = selected->GetComponentByClass<UTargetingComponent>();
			auto actions = selected->GetComponentByClass<UActionsComponent>();

			if (targeting && actions) {
				targeting->DetermineTargetData();
				actions->SendActionsToUI();
			}
			else {
				UDebugMessages::LogError(this, "failed once environment toggle moved");
			}
		}
	}
}


void UToggleMovementComponent::Setup(UStaticMeshComponent* staticMesh, FVector location1, FVector location2) {
	Location1 = location1;
	Location2 = location2;
	MovingMesh = staticMesh;
}
void UToggleMovementComponent::ActivateToggle() {
	auto dist = FVector::Dist(Location1, MovingMesh->GetRelativeLocation());
	if (dist < 5)	MovingToLocation = Location2;
	else			MovingToLocation = Location1;

	SetComponentTickEnabled(true);
}

void UToggleMovementComponent::EditorToggle() {
	ActivateToggle();
}