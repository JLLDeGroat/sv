// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionManager.h"
#include "../TargetAction.h"
#include "../LeftClickAction.h"
#include "../RightClickAction.h"
#include "../GrenadeActionComponent.h"
#include "../ActivateToggleAction.h"
#include "../SleepAction.h"
#include "../ReloadAction.h"
#include "../CycleTargetAction.h"
#include "../OverwatchAction.h"
#include "../ExtractionAction.h"
#include "../PickupAction.h"
#include "../SwapWeaponAction.h"
#include "../HealthKitUseComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UActionManager::UActionManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	TargetActionComponent = CreateDefaultSubobject<UTargetAction>(TEXT("TargetAction"));
	LeftClickActionComponent = CreateDefaultSubobject<ULeftClickAction>(TEXT("LeftClickAction"));
	RightClickActionComponent = CreateDefaultSubobject<URightClickAction>(TEXT("RightClickAction"));
	GrenadeActionComponent = CreateDefaultSubobject<UGrenadeActionComponent>(TEXT("GrenadeAction"));
	ActivateToggleActionComponent = CreateDefaultSubobject<UActivateToggleAction>(TEXT("ToggleAction"));
	SleepActionComponent = CreateDefaultSubobject<USleepAction>(TEXT("SleepAction"));
	ReloadActionComponent = CreateDefaultSubobject<UReloadAction>(TEXT("ReloadAction"));
	CycleTargetComponent = CreateDefaultSubobject<UCycleTargetAction>(TEXT("CycleTargetAction"));
	OverwatchComponent = CreateDefaultSubobject<UOverwatchAction>(TEXT("OverwatchAction"));
	ExtractionComponent = CreateDefaultSubobject<UExtractionAction>(TEXT("ExtractionAction"));
	PickupActionComponent = CreateDefaultSubobject<UPickupAction>(TEXT("PickupAction"));
	SwapActionComponent = CreateDefaultSubobject<USwapWeaponAction>(TEXT("SwapAction"));
	HealthKitUseComponent = CreateDefaultSubobject<UHealthKitUseComponent>(TEXT("HealthKitAction"));
}


// Called when the game starts
void UActionManager::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UActionManager::DoActionFromUI(EActionType actionType) {
	switch (actionType) {
	case EActionType::AT_Sleep:
	{
		SleepActionComponent->DoAction();
	}
	break;
	case EActionType::AT_Shoot:
	{
		TargetActionComponent->DoAction();
	}
	break;
	case EActionType::AT_Grenade:
	{
		GrenadeActionComponent->DoAction();
	}
	break;
	case EActionType::AT_Reload:
	{
		ReloadActionComponent->DoAction();
	}
	break;
	case EActionType::AT_Overwatch:
	{
		OverwatchComponent->DoAction();
	}
	break;
	case EActionType::AT_Extract:
	{
		ExtractionComponent->DoAction();
	}
	break;
	case EActionType::AT_Pickup:
	{
		PickupActionComponent->DoAction();
	}
	break;
	case EActionType::AT_Interact:
	{
		ActivateToggleActionComponent->DoAction();
	}
	break;
	case EActionType::AT_SwapWeapon:
	{
		SwapActionComponent->DoAction();
	}
	break;
	case EActionType::AT_BasicHealthKit:
	{
		HealthKitUseComponent->DoBasicHealth();
	}
	break;
	case EActionType::AT_LargeHealthKit:
	{
		HealthKitUseComponent->DoLargeHealth();
	}
	break;
	case EActionType::AT_AnditodeKit:
	{
		HealthKitUseComponent->DoAntidote();
	}
	break;
	case EActionType::AT_FlameRetardentKit:
	{
		HealthKitUseComponent->DoFlameRetardent();
	}
	break;
	default:
	{
		UDebugMessages::LogDisplay(this, "Failed to get an action");
	}
	}

}


UTargetAction* UActionManager::GetTargetAction() {
	return TargetActionComponent;
}
ULeftClickAction* UActionManager::GetLeftClickAction() {
	return LeftClickActionComponent;
}
URightClickAction* UActionManager::GetGetRightClickAction() {
	return RightClickActionComponent;
}
UGrenadeActionComponent* UActionManager::GetGrenadeAction() {
	return GrenadeActionComponent;
}
UActivateToggleAction* UActionManager::GetActivateToggleAction() {
	return ActivateToggleActionComponent;
}
USleepAction* UActionManager::GetSleepAction() {
	return SleepActionComponent;
}
UReloadAction* UActionManager::GetReloadAction() {
	return ReloadActionComponent;
}
UCycleTargetAction* UActionManager::GetCycleTargetAction() {
	return CycleTargetComponent;
}
UOverwatchAction* UActionManager::GetOverwatchAction() {
	return OverwatchComponent;
}
USwapWeaponAction* UActionManager::GetSwapWeaponAction() {
	return SwapActionComponent;
}