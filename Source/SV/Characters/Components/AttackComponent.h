// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "../../Enums/ECharacterEnums.h"
#include "AttackComponent.generated.h"

class ISvChar;
class UBaseRunnable;
/**
 * 
 */
UCLASS()
class SV_API UAttackComponent : public UAnimAccessComponent
{
	GENERATED_BODY()
	
public:

	UAttackComponent(const FObjectInitializer& ObjectInitializer);

	void TryAttackTarget(FVector sourceGridLocation, TScriptInterface<ISvChar> targetCharacter, bool bIsRange = true);
	void TryAttackLocation(FVector sourceGridLocation, FVector location, float locationRadius, bool bIsRange = true);

	void ReturnCharacterAnimationSpeedsToNormal();

	FVector GetCurrentTargetLocation() const;

	void UpdateCurrentAttackState(EAttackState attackState);

	TScriptInterface<ISvChar> GetCurrentTargetCharacter();

	float GetCurrentAttackRandomRadius() const;
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

	UPROPERTY() TScriptInterface<ISvChar> CurrentTargetCharacter;

	EAttackType DetermineAttackStateFromDirection(FVector currentGridLoc, FVector movementLoc, FVector targetLoc);

	UPROPERTY() UBaseRunnable* PostShootRunnable;

	UPROPERTY() float CurrentAttackRandomRadius;
};
