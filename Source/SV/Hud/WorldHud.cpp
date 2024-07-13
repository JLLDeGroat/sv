// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldHud.h"
#include "World/MainWorldWidget.h"

void AWorldHud::BeginPlay() {
	Super::BeginPlay();

	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/World/MainWorldWidget_Bp.MainWorldWidget_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UMainWorldWidget>())
	{
		auto hudWidget = CreateWidget<UMainWorldWidget>(GetWorld(), hudUIWidgetClass);
		hudWidget->AddToViewport();
		MainWorldWidget = hudWidget;
	}
}