// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter.h"
#include "ZombieGrunt.generated.h"

class UGridMovementComponent;
class UHitCapsuleComponent;
class UDamageRecieveComponent;
/**
 * 
 */
UCLASS()
class SV_API AZombieGrunt : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AZombieGrunt(const FObjectInitializer& ObjectInitializer);

protected:

	UGridMovementComponent* GetGridMovementComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UGridMovementComponent* GridMovementComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UHitCapsuleComponent* BodyHitComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDamageRecieveComponent* DamageRecieveComponent;
	
};
