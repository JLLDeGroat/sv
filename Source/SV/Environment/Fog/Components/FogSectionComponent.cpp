// Fill out your copyright notice in the Description page of Project Settings.


#include "FogSectionComponent.h"
#include "../../..//Utilities/SvUtilities.h"

void UFogSectionComponent::SetMaterialInstance(UMaterialInstanceDynamic* instance) {
	MaterialInstance = instance;
	MaterialInstance->SetScalarParameterValue(FName(USvUtilities::GetFogVariableName()), USvUtilities::GetFogValue());
	SetMaterial(0, MaterialInstance);
}

void UFogSectionComponent::SetAsNoFog() {
	MaterialInstance->SetScalarParameterValue(FName(USvUtilities::GetFogVariableName()), USvUtilities::GetNoFogValue());
}
void UFogSectionComponent::SetAsFog() {
	MaterialInstance->SetScalarParameterValue(FName(USvUtilities::GetFogVariableName()), USvUtilities::GetFogValue());
}
void UFogSectionComponent::SetAsFullFog() {
	MaterialInstance->SetScalarParameterValue(FName(USvUtilities::GetFogVariableName()), USvUtilities::GetFullFogValue());
}