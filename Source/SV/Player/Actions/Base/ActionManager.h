// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../Enums/ECharacterEnums.h"
#include "ActionManager.generated.h"

class UTargetAction;
class ULeftClickAction;
class URightClickAction;
class UGrenadeActionComponent;
class UActivateToggleAction;
class USleepAction;
class UReloadAction;
class UCycleTargetAction;
class UOverwatchAction;
class UExtractionAction;
class UPickupAction;
class USwapWeaponAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UActionManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActionManager();

	UTargetAction* GetTargetAction();
	ULeftClickAction* GetLeftClickAction();
	URightClickAction* GetGetRightClickAction();
	UGrenadeActionComponent* GetGrenadeAction();
	UActivateToggleAction* GetActivateToggleAction();
	USleepAction* GetSleepAction();
	UReloadAction* GetReloadAction();
	UCycleTargetAction* GetCycleTargetAction();
	UOverwatchAction* GetOverwatchAction();
	USwapWeaponAction* GetSwapWeaponAction();

	void DoActionFromUI(EActionType actionType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTargetAction* TargetActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ULeftClickAction* LeftClickActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) URightClickAction* RightClickActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UGrenadeActionComponent* GrenadeActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UActivateToggleAction* ActivateToggleActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) USleepAction* SleepActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UReloadAction* ReloadActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCycleTargetAction* CycleTargetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UOverwatchAction* OverwatchComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UExtractionAction* ExtractionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UPickupAction* PickupActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) USwapWeaponAction* SwapActionComponent;
};
