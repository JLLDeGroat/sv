// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UReloadAction::UReloadAction(const FObjectInitializer& ObjectInitializer)
	: UBaseActionComponent(ObjectInitializer) {

}

void UReloadAction::DoAction() {
	UDebugMessages::LogError(this, "TODO");
}