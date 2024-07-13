// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Enums/EEquipmentEnums.h"
#include "EquipmentComponent.generated.h"

class AEquipment;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

	void UpdateActorVisibility(bool value);

	TArray<AEquipment*> GetAllMeleeEquipment();

	AEquipment* GetPrimaryEquipment();

	void EquipPrimary(EGun gunType);
	void AttachEquipmentToSocket(EAttachType attachmentType, AEquipment* equipment, FString socketName);

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
