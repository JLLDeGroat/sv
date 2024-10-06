// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "LeftClickAction.generated.h"

class USelectionManager;
class UPawnCameraComponent;
class UCameraComponent;
class APlayerController;
class UHudDelegates;
class UCameraOverlapComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API ULeftClickAction : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULeftClickAction(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	void DoAction();
	void DoWorldAction();


private:

	void WhileGunTargetActive(UPawnCameraComponent* cameraComponent, UCameraComponent* camera, UCameraOverlapComponent* cameraOverlap, APlayerController* controller, UHudDelegates* hudDelegates);
	void WhileThrowTargetActive(UPawnCameraComponent* cameraComponent);
	void WhileOverwatchActive(UPawnCameraComponent* cameraComponent, APlayerController* controller);
	void WhileToolUseActive(APlayerController* controller);

	void WhileGeneric(FHitResult* hit, UHudDelegates* hudDelegates);

};
