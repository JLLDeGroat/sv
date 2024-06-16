// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicGameModeWidget.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../Runnables/ClassicGameMapGenerationRunnable.h"

void UClassicGameModeWidget::NativeConstruct() {
	Super::NativeConstruct();
	CurrentMapGrid = FMapGridData(12, 8);
}

void UClassicGameModeWidget::StartGeneration() {
	//GenThread = NewObject<UClassicGameMapGenerationRunnable>()
	//	->InsertVariables(this, GetGridData())
	//	->Initialise(GetWorld())
	//	->Begin();
}

FMapGridData* UClassicGameModeWidget::GetGridData() {
	return &CurrentMapGrid;
}

void UClassicGameModeWidget::SetGridRoute(TArray<FVector2D> arr) {
	Route = arr;
	//SetGridDetails(Route, true, false);
}

void UClassicGameModeWidget::SetGridOffshoots(TArray<FVector2D> arr) {
	Offshoots = arr;
	//UpdateGridWithOffshoots(Offshoots);
}