// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthKitUseIndicator.h"
#include "../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/IndicatorClickComponent.h"
#include "Components/IndicatorActivatorComponent.h"
#include "Components/IndicatorLinkComponent.h"

AHealthKitUseIndicator::AHealthKitUseIndicator() {
	ActivatorIndicator->SetIndicatorType(EIndicatorType::IT_Extract);
	SetMaterialColour(FLinearColor(.26f, .63f, .71f, .05f));


	ClickComponent = CreateDefaultSubobject<UIndicatorClickComponent>(TEXT("ClickIndicator"));
	LinkComponent = CreateDefaultSubobject<UIndicatorLinkComponent>(TEXT("LinkComponent"));
	
}