// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Enums/EEquipmentEnums.h"
#include "EquipmentDetailsComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UEquipmentDetailsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEquipmentDetailsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetIsMelee() const;
	void SetIsMelee(bool val);

	bool GetIsRange() const;
	void SetIsRange(bool val);

	int GetMinBaseDamage() const;
	void SetMinBaseDamage(int amount);

	int GetMaxBaseDamage() const;
	void SetMaxBaseDamage(int amount);

	float GetCritMultiplier() const;
	void SetCritMultiplier(float amount);

	void SetApCost(int amount);
	int GetApCost() const;

	bool GetCanOverwatch();
	void SetCanOverwatch(bool val);

	void SetOverwatchApCost(int amount);
	int GetOverwatchApCost() const;

	void SetIsPrimaryEquipment(bool val);
	bool GetIsPrimaryEquipment();

	void SetIsSecondaryEquipment(bool val);
	bool GetIsSecondaryEquipment();

	void SetAccuracy(float val);
	float GetAccuracy();

	void SetBaseAccuracy(float val);
	float GetBaseAccuracy();

	void SetAccuracyDecay(float val);
	float GetAccuracyDecay();

	void SetMaxAccuracyDeviation(float maxDeviation);
	float GetMaxAccuracyDeviation();

	void RemoveFromRounds(int amount);
	int GetRounds();
	void FillRounds();

	int GetMaxRounds();
	void SetMaxRounds(int maxRounds);

	int GetReloadApCost();
	void SetReloadApCost(int cost);

	bool CanReloadWeapon();

	void SetGunType(EGun gunType);
	EGun GetGunType();
	bool GetIsGun();

	void SetEquipmentId(FGuid id);
	FGuid GetEquipmentId();

	EAttachType GetEquipSocket();
	void SetEquipSocket(EAttachType equipmentSocket);

	EAttachType GetHolsterAttachType();
	void SetHolsterAttachType(EAttachType attachType);

	FVector GetRelativeScaleOnEquip();
	void SetRelativeScaleOnEquip(FVector loc);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsRange;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsMelee;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) int BaseDamageMinimum;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int BaseDamageMaximum;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float CritMultiplier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bCanOverwatch;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) int ApCost;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int OverwatchApCost;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int ReloadApCost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float BaseAccuracy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Accuracy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float AccuracyDecay;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float MaxAccuracyDeviation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsPrimaryEquipment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsSecondaryEquipment;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Rounds;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int MaxRounds;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) EGun GunType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FGuid EquipmentId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) EAttachType HolsterType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) EAttachType EquipSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector RelativeScaleOnEquip;

};
