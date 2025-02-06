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
class UHealthAndStatusWidgetComponent;
class UActionsComponent;
class UCharacterTileUIComponent;
class UDestroyComponent;
class UPickupResourceComponent;
class UDropResourceComponent;
class UInventoryComponent;
class UHitCapsuleComponent;
class UHitBoxComponent;
class UCharacterCaptureComponent;
class UHealthKitsComponent;
class UFogHandlerComponent;
class UClimbLadderComponent;
class UDeviantDirectiveComponent;

UCLASS()
class SV_API ASolder : public ABaseCharacter, public IMovable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASolder(const FObjectInitializer &ObjectInitializer);

	// override from base class
	virtual bool TryGetAsMoveable(TScriptInterface<IMovable> &Moveable) override;

	virtual UGridMovementComponent *GetGridMovementComponent() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UGridMovementComponent *GridMovementComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTargetingComponent *TargetingComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UEquipmentComponent *EquipmentComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAttackComponent *AttackComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDamageRecieveComponent *DamageRecieveComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimSpeedComponent *AnimSpeedComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UThrowableComponent *ThrowableComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UActivateTogglesComponent *ActivateToggleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UVaultObstacleComponent *VaultingComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UHealthAndStatusWidgetComponent *HealthAndStatusComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UActionsComponent *ActionsComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCharacterTileUIComponent *CharacterTileUIComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDestroyComponent *DestroyComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPickupResourceComponent *PickupResourceComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInventoryComponent *InventoryComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDropResourceComponent *DropResourceComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCharacterCaptureComponent *CharacterCaptureComponent;

	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UHitCapsuleComponent *LeftArmHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UHitCapsuleComponent *LeftForeArmHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UHitCapsuleComponent *RightArmHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UHitCapsuleComponent *RightForeArmHitComponent;

	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UHitBoxComponent *UpperTorsoHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UHitBoxComponent *LowerTorsoHitComponent;

	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UHitCapsuleComponent *LeftLegHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UHitCapsuleComponent *RightLegHitComponent;

	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UHitCapsuleComponent *LeftUpperLegHitComponent;
	UPROPERTY(BlueprintreadWrite, EditAnywhere)
	UHitCapsuleComponent *RightUpperLegHitComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UHitCapsuleComponent *HeadHitComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UHealthKitsComponent *HealthKitsComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UFogHandlerComponent *FogHandler;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClimbLadderComponent *ClimbLadderComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDeviantDirectiveComponent *DDComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
