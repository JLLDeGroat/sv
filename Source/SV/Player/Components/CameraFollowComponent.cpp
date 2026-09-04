// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraFollowComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "DrawDebugHelpers.h"
#include "../../Utilities/SvUtilities.h"

UCameraFollowComponent::UCameraFollowComponent(const FObjectInitializer &ObjectInitializer) : UActorComponent(ObjectInitializer)
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCameraFollowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    auto desiredLocation = FollowComponent ? FollowComponent->GetComponentLocation() : FollowActor ? FollowActor->GetActorLocation()
                                                                                                   : FVector::ZeroVector;

    if (desiredLocation == FVector::ZeroVector)
        return UDebugMessages::LogError(this, "no follow actor or component set for camera follow component");

    desiredLocation = desiredLocation - CameraOffsetVector;
    desiredLocation.Z = CameraComponent->GetComponentLocation().Z;

    auto newLocation = UKismetMathLibrary::VInterpTo_Constant(CameraComponent->GetComponentLocation(), desiredLocation, DeltaTime, 300);
    CameraComponent->SetWorldLocation(newLocation);
}

void UCameraFollowComponent::BeginPlay()
{
    Super::BeginPlay();
    SetComponentTickEnabled(false);
}

void UCameraFollowComponent::SetFollowComponent(UPrimitiveComponent *componentToFollow)
{
    FollowComponent = componentToFollow;
    if (CalculateCameraOffset(FollowComponent->GetComponentLocation()))
        SetComponentTickEnabled(true);
}
void UCameraFollowComponent::SetFollowActor(AActor *actorToFollow)
{
    FollowActor = actorToFollow;

    if (CalculateCameraOffset(FollowActor->GetActorLocation()))
        SetComponentTickEnabled(true);
}
void UCameraFollowComponent::CompleteFollow()
{
    FollowComponent = nullptr;
    FollowActor = nullptr;
    SetComponentTickEnabled(false);
}

void UCameraFollowComponent::SetCameraComponent(UCameraComponent *cameraComponent)
{
    CameraComponent = cameraComponent;
}

bool UCameraFollowComponent::CalculateCameraOffset(FVector locationToFollow)
{
    FHitResult cameraFacingFloorHit;
    GetWorld()->LineTraceSingleByChannel(cameraFacingFloorHit, CameraComponent->GetComponentLocation(), CameraComponent->GetComponentLocation() + (CameraComponent->GetForwardVector() * 5000), USvUtilities::GetFloorTargetChannel());

    if (cameraFacingFloorHit.GetActor())
        CameraOffsetVector = cameraFacingFloorHit.ImpactPoint - locationToFollow;
    else
    {
        UDebugMessages::LogError(this, "camera follow component could not find floor in front of camera, cannot set follow actor");
        return false;
    }
    return true;
}