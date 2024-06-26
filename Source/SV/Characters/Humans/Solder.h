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
class UAnimSpeedComponent;
class UThrowableComponent;
class UActivateTogglesComponent;
class UVaultObstacleComponent;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UGridMovementComponent* GridMovementComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTargetingComponent* TargetingComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UEquipmentComponent* EquipmentComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UAttackComponent* AttackComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDamageRecieveComponent* DamageRecieveComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UAnimSpeedComponent* AnimSpeedComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UThrowableComponent* ThrowableComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UActivateTogglesComponent* ActivateToggleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UVaultObstacleComponent* VaultingComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
