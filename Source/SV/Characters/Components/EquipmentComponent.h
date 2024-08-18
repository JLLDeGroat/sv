// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "../../Enums/EEquipmentEnums.h"
#include "EquipmentComponent.generated.h"

class AEquipment;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UEquipmentComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent(const FObjectInitializer& ObjectInitializer);

	void UpdateActorVisibility(bool value);

	TArray<AEquipment*> GetAllMeleeEquipment();

	AEquipment* GetPrimaryEquipment();

	void EquipPrimary(EGun gunType);
	void AttachEquipmentToSocket(EAttachType attachmentType, AEquipment* equipment, FString socketName);

	void ReloadEquipment();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FireEquippedGun();
	int GetActionPointsNeededToUseEquipment();
private:
	UPROPERTY() TArray<AEquipment*> Equipment;
};
