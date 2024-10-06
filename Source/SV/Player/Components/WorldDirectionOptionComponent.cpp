// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldDirectionOptionComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../../World/DirectionOptionActor.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UWorldDirectionOptionComponent::UWorldDirectionOptionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldDirectionOptionComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningController = GetOwner<APlayerController>();
}


// Called every frame
void UWorldDirectionOptionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult Hit;
	OwningController->GetHitResultUnderCursor(USvUtilities::GetWorldSelectChannel(), false, Hit);

	if (ADirectionOptionActor* directionActor = Cast<ADirectionOptionActor>(Hit.GetActor())) {
		if (!CurrentOptionActor) {
			CurrentOptionActor = directionActor;
			CurrentOptionActor->ActivateArrowEmission();
		}
		else if (CurrentOptionActor != Hit.GetActor()) {
			CurrentOptionActor->DeactivateArrowEmission();
			CurrentOptionActor = directionActor;
			CurrentOptionActor->ActivateArrowEmission();
		}
	}
}

