// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Enums/ECharacterEnums.h"
#include "PawnCameraComponent.generated.h"

class UCameraComponent;
class UPostProcessComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UPawnCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPawnCameraComponent();

	void UpdateCameraState(ECameraState cameraState, FVector moveToLocation = FVector::ZeroVector, FVector lookAtLocation = FVector::ZeroVector, bool overrideCameraMovements = false, bool doNotSetReturn = false);
	ECameraState GetCurrentCameraState();

	void DoCinematicAttackCameraMovement(AActor* attacker, AActor* target);
	void DoOverwatchCinematicAttackCameraMovement(AActor* attacker, AActor* target);
	void SetOverwatchReturnLocation(FVector loc);

	void SetDefaultCameraOffset(FVector defaultValue);

	void SetToDeathWatchCamera(AActor* deathActor);
	void SetEndDeathWatchCamera();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) ECameraState CurrentCameraState;
private:

	void CinematicAttackCameraMovement(AActor* attacker, AActor* target);

	UPROPERTY() FVector MoveToLocation;
	UPROPERTY() FVector LookToLocation;

	UPROPERTY() FVector ReturnLocation;

	UPROPERTY() FVector CurrentMoveTo;
	UPROPERTY() FVector CurrentRotateToLocation;

	UPROPERTY() bool CurrentlyMoving;

	UPROPERTY() FVector DefaultCameraOffset;

	UPROPERTY() UCameraComponent* CameraComponent;

	bool ShoudUseSetRotation() const;

	UPROPERTY() AActor* CinematicActorAttacker;
	UPROPERTY() AActor* CinematicActorTarget;

	UPROPERTY() AActor* DeathActor;
	UPROPERTY() UPrimitiveComponent* DeathComponentToWatch;
	UPROPERTY() FVector DeathWatchFromLocation;

	bool GetValidCinematicLocation(FVector& location);
	bool GetValidCinematicLocation(AActor* actor, FVector& location);

	UPROPERTY() float DefaultDepthOfFieldFstopValue = 0.0f;
	UPROPERTY() float DefaultDepthOfFieldSensorWidth = 0.0f;
	UPROPERTY() float DefaultDepthOfFieldFocalDistance = 0.0f;


	void AttemptToAlterAttackerStatusWidgetVisibility(bool val);
};
