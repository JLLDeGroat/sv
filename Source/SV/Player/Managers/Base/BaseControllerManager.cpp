// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseControllerManager.h"
#include "../../GamePlayerController.h"

UBaseControllerManager::UBaseControllerManager(const FObjectInitializer& ObjectInitializer) : UObject(ObjectInitializer) {

}

void UBaseControllerManager::SetOwningController(AGamePlayerController* controller) {
	OwningController = controller;
}