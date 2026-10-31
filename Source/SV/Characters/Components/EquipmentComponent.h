// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "../../Enums/EEquipmentEnums.h"
#include "../../Enums/ECharacterEnums.h"
#include "EquipmentComponent.generated.h"

SV_API DECLARE_LOG_CATEGORY_EXTERN(SVLogEquipmentComponent, Log, All);

class AEquipment;
class AClothing;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UEquipmentComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEquipmentComponent(const FObjectInitializer &ObjectInitializer);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void FireEquippedGun();
	int GetActionPointsNeededToUseEquipment();

	void TryActivateMainEquipment();
	void TryDeactivateMainEquipment();

	void UpdateActorVisibility(bool value);

	TArray<AEquipment *> GetAllMeleeEquipment();
	TArray<AEquipment *> GetAllEquipment();
	TArray<AClothing *> GetAllClothing();

	AEquipment *GetPrimaryEquipment();
	AEquipment *GetSecondaryEquipment();

	bool GetCanSwapWeapon();
	void SwapWeapon();
	void FinishSwapWeapon();

	void HolsterCurrentMainEquipment();
	void UnholsterNewMainEquipment();

	void EquipPrimary(EGun gunType);
	void EquipSecondary(EGun gunType);
	void AttachEquipmentToSocket(EAttachType attachmentType, AEquipment *equipment);

	void EquipClothing(EClothingType clothingType, EClothingGrade clothingGrade);

	void UnEquip(AEquipment *equipment);

	void ReloadEquipment();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AEquipment *CurrentMainEquipment;
	UPROPERTY()
	TArray<AEquipment *> Equipment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AClothing *> Clothing;

	ECharacterAnimState GetAnimStateFromGunType(EGun gunType);

	UPROPERTY() bool TestValue;
};
