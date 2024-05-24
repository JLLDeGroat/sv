// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter.h"
#include "../../Interfaces/Movable.h"
#include "ZombieGrunt.generated.h"

class UGridMovementComponent;
class UHitCapsuleComponent;
class UDamageRecieveComponent;
class UEquipmentComponent;
class USkillsComponent;
class UAttackComponent;

/**
 * 
 */
UCLASS()
class SV_API AZombieGrunt : public ABaseCharacter, public IMovable
{
	GENERATED_BODY()

public:

	AZombieGrunt(const FObjectInitializer& ObjectInitializer);

protected:

	virtual UGridMovementComponent* GetGridMovementComponent() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UGridMovementComponent* GridMovementComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UHitCapsuleComponent* BodyHitComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDamageRecieveComponent* DamageRecieveComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UEquipmentComponent* EquipmentComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USkillsComponent* SkillComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UAttackComponent* AttackComponent;
	
};
