// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHud.h"
#include "GameHuds/MainGameHud.h"

void AGameHud::BeginPlay() {
	Super::BeginPlay();

	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/MainGameHud_Bp.MainGameHud_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UMainGameHud>())
	{
		auto hudWidget = CreateWidget<UMainGameHud>(GetWorld(), hudUIWidgetClass);
		hudWidget->AddToViewport();
		MainGameHud = hudWidget;
	}
}