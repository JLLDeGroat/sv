// Fill out your copyright notice in the Description page of Project Settings.

#include "DeviantDirectiveComponent.h"
#include "OnGunFire/TrickleDownBallistics.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UDeviantDirectiveComponent::UDeviantDirectiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDeviantDirectiveComponent::AddDirectiveOrDeviant(TSubclassOf<UBaseDD> baseDD)
{
	UBaseDD *newDirective = NewObject<UBaseDD>(this, baseDD);

	for (int i = 0; i < Components.Num(); i++)
		if (Components[i]->StaticClass() == newDirective->StaticClass())
		{
			newDirective = nullptr;
			UDebugMessages::LogDisplay(this, "identical dd added, ignoring the same");
			return;
		}

	Components.Emplace(newDirective);
}

TArray<UOnGunFireBase *> UDeviantDirectiveComponent::GetGunFireDDs()
{
	TArray<UOnGunFireBase *> result;
	for (UBaseDD *Component : Components)
		if (UOnGunFireBase *GunFireDD = Cast<UOnGunFireBase>(Component))
			result.Emplace(GunFireDD);

		return result;
}