// Fill out your copyright notice in the Description page of Project Settings.


#include "RouteDataManager.h"

URouteDataManager::URouteDataManager() {

}

void URouteDataManager::SetCurrentRoute(TArray<FVector2D> arr) {
	CurrentRoute = arr;
}
void URouteDataManager::SetCurrentOffshoots(TArray<FVector2D> arr) {
	Offshoots = arr;
}

TArray<FVector2D> URouteDataManager::GetCurrentRoute() {
	return CurrentRoute;
}
TArray<FVector2D> URouteDataManager::GetCurrentOffshoots() {
	return Offshoots;
}

void URouteDataManager::SetCurrentLocationOnRoute(FVector2D loc) {
	CurrentLocationOnRoute = loc;
}
FVector2D URouteDataManager::GetCurrentLocationOnRoute() {
	return CurrentLocationOnRoute;
}