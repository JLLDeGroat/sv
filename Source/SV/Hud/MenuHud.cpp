// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHud.h"
#include "MenuHuds/MainMenuLayoutWidget.h"

void AMenuHud::BeginPlay() {
	Super::BeginPlay();
	
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/Menu/MenuHudWidget_Bp.MenuHudWidget_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UMainMenuLayoutWidget>())
	{
		auto hudWidget = CreateWidget<UMainMenuLayoutWidget>(GetWorld(), hudUIWidgetClass);
		hudWidget->AddToViewport();
		MenuLayoutWidget = hudWidget;
	}
}