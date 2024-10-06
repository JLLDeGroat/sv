// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseConstruct.h"
#include "../../Interfaces/Movable.h"
#include "RifleConstruct.generated.h"


class UGridMovementComponent;
class UHitCapsuleComponent;
class UHitBoxComponent;
class UDamageRecieveComponent;
class UEquipmentComponent;
class USkillsComponent;
class UAttackComponent;
class UAnimSpeedComponent;
class UHealthAndStatusWidgetComponent;
class UAIComponent;
class UDropResourceComponent;
class USpawnInComponent;
class UTargetingComponent;
/**
 *
 */
UCLASS()
class SV_API ARifleConstruct : public ABaseConstruct, public IMovable
{
	GENERATED_BODY()

public:
	ARifleConstruct(const FObjectInitializer& ObjectInitializer);

	virtual UGridMovementComponent* GetGridMovementComponent() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTargetingComponent* TargetingComponent;

};
