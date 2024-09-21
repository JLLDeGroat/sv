// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter.h"
#include "../../Interfaces/Movable.h"
#include "GruntConstruct.generated.h"


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
/**
 * 
 */
UCLASS()
class SV_API AGruntConstruct : public ABaseCharacter, public IMovable
{
	GENERATED_BODY()
public:

	AGruntConstruct(const FObjectInitializer& ObjectInitializer);

protected:

	virtual UGridMovementComponent* GetGridMovementComponent() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UGridMovementComponent* GridMovementComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDamageRecieveComponent* DamageRecieveComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UEquipmentComponent* EquipmentComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USkillsComponent* SkillComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UAttackComponent* AttackComponent;

	//Hit Boxes

	UPROPERTY(BlueprintreadWrite, EditAnywhere) UHitCapsuleComponent* LeftArmHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere) UHitCapsuleComponent* LeftForeArmHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere) UHitCapsuleComponent* RightArmHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere) UHitCapsuleComponent* RightForeArmHitComponent;

	UPROPERTY(BlueprintreadWrite, EditAnywhere) UHitBoxComponent* UpperTorsoHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere) UHitBoxComponent* LowerTorsoHitComponent;

	UPROPERTY(BlueprintreadWrite, EditAnywhere) UHitCapsuleComponent* LeftLegHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere) UHitCapsuleComponent* RightLegHitComponent;

	UPROPERTY(BlueprintreadWrite, EditAnywhere) UHitCapsuleComponent* LeftUpperLegHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere) UHitCapsuleComponent* RightUpperLegHitComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UHitCapsuleComponent* HeadHitComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UAnimSpeedComponent* AnimSpeedComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UHealthAndStatusWidgetComponent* HealthAndStatusComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UAIComponent* AiComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDropResourceComponent* DropResourceComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USpawnInComponent* SpawnInComponent;
};
