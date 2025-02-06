// Fill out your copyright notice in the Description page of Project Settings.


#include "TraversalLocationsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UTraversalLocationsComponent::UTraversalLocationsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTraversalLocationsComponent::SetStartGridLocation(FVector start)
{
	StartLocation = start;
}

void UTraversalLocationsComponent::SetEndGridLocation(FVector end)
{
	EndLocation = end;
}

FVector UTraversalLocationsComponent::GetStartGridLocation()
{
	return StartLocation;
}

FVector UTraversalLocationsComponent::GetEndGridLocation()
{
	return EndLocation;
}

