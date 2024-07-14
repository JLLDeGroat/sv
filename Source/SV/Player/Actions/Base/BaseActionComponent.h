// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../GamePlayerController.h"
#include "../../../Enums/ECharacterEnums.h"
#include "BaseActionComponent.generated.h"

class USelectionManager;
class UControlManager;
class AGamePlayerController;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UBaseActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseActionComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	AGamePlayerController* GetOwningController();

	void ResetActionEffects();

	bool IsInValidCameraState(ECameraState currentCameraState);

	void GetTargetLocation(FHitResult& hit, FVector& targetLocation, UCameraComponent* cameraComp);


	UPROPERTY() USelectionManager* SelectionManager;
	UPROPERTY() UControlManager* ControlManager;

	UPROPERTY() AGamePlayerController* PlayerController;

	UPROPERTY() TArray<ECameraState> ValidCameraStates;


};
