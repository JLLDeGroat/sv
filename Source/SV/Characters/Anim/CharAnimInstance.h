// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../../Enums/ECharacterEnums.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharAnimInstance.generated.h"

class ABaseCharacter;
class UBaseRunnable;
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
	void SetIsVaulting(bool val);
	void SetIsCrouching(bool val);
	void SetIsReloading(bool val);
	void SetIsTakenDamage(bool val);
	void SetIsAiActive(bool val);
	void SetIsPickingUp(bool val);
	void SetCharacterAnimState(ECharacterAnimState animState);

	void SetIsSpawningFromGround(bool val);

	void SetIsHealingSelf(bool val);
	void SetIsHealingAlly(bool val);
	void SetIsSuiciding(bool val);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnGunPreFireActivate();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnGunPostFireDeactivate();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnGunFire();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnFinishFire();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnFinishFire_PostDelay();
	FTimerHandle OnFinishFireHandle;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnMeleeHit();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnFinishMelee();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnHolsterWeapon();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnUnholsteredWeapon();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnGrabbedThrowable();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnReleasedThrowable();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnReloadFinish();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnFinishVault();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnFinishTakenDamage();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnInitializePickup();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnFinishPickup();

	void UpdateAnimPlayRate(float newRate);


	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void SpawningFromGroundStartMovingUp();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void SpawningFromGroundFinishMovingUp();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void SpawningFromGroundAlterMovingUpSpeed(float speed);


	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnHolsterCurrentWeapon();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnUnHolsterNewWeapon();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnFinishWeaponSwapping();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnHealingComplete();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnSpawnHealthKit();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnDespawnHealthKit();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void SetHealthKitActivation(bool val);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe)) void OnSuicideExplosion();
protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsAttacking;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsThrowing;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) EAttackType AttackType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) ECharacterAnimState CharacterAnimState;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float AnimPlayRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsCrouching;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsVaulting;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsReloading;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bHasTakenDamage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsAiActive;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsPickingUp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsHealingSelf;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsHealingAlly;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bSpawningFromGround;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsSuiciding;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe)) bool GetIsHealing();

private:

	UPROPERTY() UBaseRunnable* BaseRunnable;

};
