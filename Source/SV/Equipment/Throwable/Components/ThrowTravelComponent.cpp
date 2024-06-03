// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowTravelComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/CapsuleComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Utilities/GridUtilities.h"
#include "ThrowExplosionComponent.h"

// Sets default values for this component's properties
UThrowTravelComponent::UThrowTravelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UThrowTravelComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}


// Called every frame
void UThrowTravelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Trajectories.Num() > 0) {
		auto nextLocation = Trajectories[0];
		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), nextLocation, DeltaTime, 650);

		GetOwner()->AddActorLocalRotation(FRotator(2, 3, 4));
		GetOwner()->SetActorLocation(newLocation);

		if (FVector::Dist(GetOwner()->GetActorLocation(), nextLocation) < 5)
			Trajectories.RemoveAt(0);
	}
	else {
		SetComponentTickEnabled(false);
		auto capsuleComponent = GetOwner()->GetComponentByClass<UCapsuleComponent>();
		if (capsuleComponent) {
			capsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			capsuleComponent->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Block);
			capsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			capsuleComponent->SetSimulatePhysics(true);

			auto lookat = UGridUtilities::FindLookAtRotation(GetOwner()->GetActorLocation(), BeginningLocation);
			auto currentLoc = GetOwner()->GetActorLocation();
			auto radialLocation = GetOwner()->GetActorLocation() + (lookat.Vector() * 50);
			capsuleComponent->AddRadialImpulse(radialLocation, 150, 1000000, ERadialImpulseFalloff::RIF_Constant);
		}

		auto explosionComponent = GetOwner()->GetComponentByClass<UThrowExplosionComponent>();
		if (explosionComponent) explosionComponent->BeginExplosion();
	}
}
void UThrowTravelComponent::SetDestination(FVector location, FVector currentLocation) {
	Destination = location;
	BeginningLocation = currentLocation;
}
void UThrowTravelComponent::BeginTravel() {

	auto owner = GetOwner()->GetOwner();
	if (owner) {
		auto skelemesh = owner->GetComponentByClass<USkeletalMeshComponent>();
		if (skelemesh) {
			GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Trajectories.Empty();

			auto currentLocation = skelemesh->GetSocketLocation(FName("RightHandSocket"));

			auto halfway = ((currentLocation + Destination) / 2);
			auto quater1 = ((currentLocation + halfway) / 2);
			auto quater3 = ((halfway + Destination) / 2);

			quater1 += FVector(0, 0, 75);
			halfway += FVector(0, 0, 100);
			quater3 += FVector(0, 0, 50);

			Trajectories.Emplace(quater1);
			Trajectories.Emplace(halfway);
			Trajectories.Emplace(quater3);
			Trajectories.Emplace(Destination);
		}
	}

	SetComponentTickEnabled(true);
}