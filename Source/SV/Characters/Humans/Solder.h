// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter.h"
#include "../../Interfaces/Movable.h"
#include "Solder.generated.h"

class UGridMovementComponent;
class UTargetingComponent;
class UEquipmentComponent;
class UAttackComponent;
class UDamageRecieveComponent;

UCLASS()
class SV_API ASolder : public ABaseCharacter, public IMovable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASolder(const FObjectInitializer& ObjectInitializer);

	//override from base class
	virtual bool TryGetAsMoveable(TScriptInterface<IMovable>& Moveable) override;

	virtual UGridMovementComponent* GetGridMovementComponent() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY() UGridMovementComponent* GridMovementComponent;
	UPROPERTY() UTargetingComponent* TargetingComponent;
	UPROPERTY() UEquipmentComponent* EquipmentComponent;
	UPROPERTY() UAttackComponent* AttackComponent;
	UPROPERTY() UDamageRecieveComponent* DamageRecieveComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
