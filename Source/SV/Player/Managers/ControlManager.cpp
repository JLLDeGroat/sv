
// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlManager.h"

UControlManager::UControlManager(const FObjectInitializer& ObjectInitializer) : UBaseControllerManager(ObjectInitializer) {
	bCanMouseDesignateSelectionDecal = true;
}

bool UControlManager::GetCanMouseDesignateSelectionDecal() {
	return bCanMouseDesignateSelectionDecal;
}
void UControlManager::SetCanMouseDesignateSelectionDecal(bool val) {
	bCanMouseDesignateSelectionDecal = val;
}