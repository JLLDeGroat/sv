// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "../../Enums/EEquipmentEnums.h"
#include "../../Enums/ECharacterEnums.h"
#include "EquipmentComponent.generated.h"

class AEquipment;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UEquipmentComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEquipmentComponent(const FObjectInitializer& ObjectInitializer);

	void UpdateActorVisibility(bool value);

	TArray<AEquipment*> GetAllMeleeEquipment();

	AEquipment* GetPrimaryEquipment();
	AEquipment* GetSecondaryEquipment();

	bool GetCanSwapWeapon();
	void SwapWeapon();
	void FinishSwapWeapon();

	void HolsterCurrentMainEquipment();
	void UnholsterNewMainEquipment();

	void EquipPrimary(EGun gunType);
	void EquipSecondary(EGun gunType);
	void AttachEquipmentToSocket(EAttachType attachmentType, AEquipment* equipment);

	void UnEquip(AEquipment* equipment);

	void ReloadEquipment();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FireEquippedGun();
	int GetActionPointsNeededToUseEquipment();

	void TryActivateMainEquipment();
	void TryDeactivateMainEquipment();
private:
	UPROPERTY() AEquipment* CurrentMainEquipment;
	UPROPERTY() TArray<AEquipment*> Equipment;

	ECharacterAnimState GetAnimStateFromGunType(EGun gunType);
};
