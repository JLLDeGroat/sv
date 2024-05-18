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


	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnGunFire();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnFinishFire();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsAttacking;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) EAttackType AttackType;
};
