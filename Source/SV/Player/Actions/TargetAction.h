// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "TargetAction.generated.h"

class ATargetingIndicatorActor;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class SV_API UTargetAction : public UBaseActionComponent
{
	GENERATED_BODY()
	
public:

	UTargetAction(const FObjectInitializer& ObjectInitializer);

	void DoAction();
	void ResetTargetingActor();

	float SetScaleAndGetTargetingRadius(FVector scale);

	float GetTargetingIndicatorRadius();

	ATargetingIndicatorActor* GetTargetingIndicatorActor();

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCameraComponent* PawnCameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ATargetingIndicatorActor* TargetingIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CurrentTargetAccuracy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CurrentTargetAccuracyDecay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CurrentTargetBaseAccuracy;

private:

	void SetCurrentEquipmentAccuracy(AActor* actor);

};
