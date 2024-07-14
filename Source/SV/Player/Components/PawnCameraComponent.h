// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Enums/ECharacterEnums.h"
#include "PawnCameraComponent.generated.h"

class UCameraComponent;
class UPostProcessComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UPawnCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPawnCameraComponent();

	void UpdateCameraState(ECameraState cameraState, FVector moveToLocation = FVector::ZeroVector, FVector lookAtLocation = FVector::ZeroVector, bool overrideCameraMovements = false);
	ECameraState GetCurrentCameraState();

	void DoCinematicAttackCameraMovement(AActor* attacker, AActor* target);

	void SetDefaultCameraOffset(FVector defaultValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	UPROPERTY() FVector MoveToLocation;
	UPROPERTY() FVector LookToLocation;
	
	UPROPERTY() FVector ReturnLocation;
	
	UPROPERTY() FVector CurrentMoveTo;
	UPROPERTY() FVector CurrentRotateToLocation;

	UPROPERTY() ECameraState CurrentCameraState;

	UPROPERTY() bool CurrentlyMoving;

	UPROPERTY() FVector DefaultCameraOffset;

	UPROPERTY() UCameraComponent* CameraComponent;

	bool ShoudUseSetRotation() const;

	UPROPERTY() AActor* CinematicActorAttacker;
	UPROPERTY() AActor* CinematicActorTarget;

	bool GetValidCinematicLocation(FVector& location);

	UPROPERTY() float DefaultDepthOfFieldFstopValue = 0.0f;
	UPROPERTY() float DefaultDepthOfFieldSensorWidth = 0.0f;
	UPROPERTY() float DefaultDepthOfFieldFocalDistance = 0.0f;
		

	void AttemptToAlterAttackerStatusWidgetVisibility(bool val);
};
