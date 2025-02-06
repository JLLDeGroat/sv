// Fill out your copyright notice in the Description page of Project Settings.

#include "FogSectionComponent.h"
#include "../../..//Utilities/SvUtilities.h"

void UFogSectionComponent::SetMaterialInstance(
    UMaterialInstanceDynamic *instance)
{
  MaterialInstance = instance;
  MaterialInstance->SetScalarParameterValue(
      FName(USvUtilities::GetFogVariableName()), USvUtilities::GetFogValue());
  SetMaterial(0, MaterialInstance);
  bIsInFog = true;
}

void UFogSectionComponent::SetAsNoFog()
{
  MaterialInstance->SetScalarParameterValue(
      FName(USvUtilities::GetFogVariableName()), USvUtilities::GetNoFogValue());
  bIsInFog = false;
}
void UFogSectionComponent::SetAsFog()
{
  MaterialInstance->SetScalarParameterValue(
      FName(USvUtilities::GetFogVariableName()), USvUtilities::GetFogValue());
  bIsInFog = true;
}
void UFogSectionComponent::SetAsFullFog()
{
  MaterialInstance->SetScalarParameterValue(
      FName(USvUtilities::GetFogVariableName()),
      USvUtilities::GetFullFogValue());
  bIsInFog = true;
}

bool UFogSectionComponent::GetIsInFog() { return bIsInFog; }

void UFogSectionComponent::InitializeFogSectionProperties(FVector loc)
{
  SetRelativeLocation(loc);
  SetRelativeScale3D(FVector(1.f, 1.f, 2.f));
  SetGenerateOverlapEvents(true);
  SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  SetCollisionObjectType(USvUtilities::GetFogCollisionObjectChannel());
  SetCollisionResponseToChannel(USvUtilities::GetFogCollisionObjectChannel(),
                                ECollisionResponse::ECR_Overlap);
  SetCanEverAffectNavigation(false);
  this->UpdateCollisionProfile();
}