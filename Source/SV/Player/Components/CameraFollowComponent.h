// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraFollowComponent.generated.h"

class UCameraComponent;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UCameraFollowComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCameraFollowComponent(const FObjectInitializer &ObjectInitializer);

    void SetFollowComponent(UPrimitiveComponent *componentToFollow);
    void SetFollowActor(AActor *actorToFollow);
    void SetCameraComponent(UCameraComponent* cameraComponent);

    void CompleteFollow();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
    virtual void BeginPlay() override;

protected:
    UPROPERTY()
    UPrimitiveComponent *FollowComponent;
    UPROPERTY()
    UCameraComponent *CameraComponent;

    UPROPERTY() FVector CameraOffsetVector;

    UPROPERTY()
    AActor *FollowActor;

    bool CalculateCameraOffset(FVector locationToFollow);
};
