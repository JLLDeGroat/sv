// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "../../Enums/ECharacterEnums.h"
#include "AttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UAttackComponent : public UAnimAccessComponent
{
	GENERATED_BODY()
	
public:

	UAttackComponent(const FObjectInitializer& ObjectInitializer);

	void TryAttackLocation(FVector sourceGridLocation, FVector location);

	FVector GetCurrentTargetLocation();

	void UpdateCurrentAttackState(EAttackState attackState);

protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY() EAttackState CurrentAttackState;

	UPROPERTY() FVector CurrentTargetLocation;

	UPROPERTY() FVector InitialLocation;
	UPROPERTY() FRotator InitialRotation;
	UPROPERTY() FVector MoveToLocation;
	UPROPERTY() FRotator AdditionalRotation;

	EAttackType DetermineAttackStateFromDirection(FVector currentGridLoc, FVector movementLoc, FVector targetLoc);

};
