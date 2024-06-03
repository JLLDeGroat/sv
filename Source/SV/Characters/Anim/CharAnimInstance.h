// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../../Enums/ECharacterEnums.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharAnimInstance.generated.h"

class ABaseCharacter;

/**
 *
 */
UCLASS()
class SV_API UCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UCharAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void UpdateSpeed(float value);
	void SetIsAttacking(bool value, EAttackType attackType = EAttackType::AT_BasicFire);
	void SetIsThrowing(bool value, EAttackType attackType = EAttackType::AT_BasicThrow);


	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnGunFire();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnFinishFire();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnMeleeHit();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnFinishMelee();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnHolsterWeapon();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnUnholsteredWeapon();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnGrabbedThrowable();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnReleasedThrowable();

	void UpdateAnimPlayRate(float newRate);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsAttacking;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsThrowing;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) EAttackType AttackType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float AnimPlayRate;
};
