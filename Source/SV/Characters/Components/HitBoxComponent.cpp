// Fill out your copyright notice in the Description page of Project Settings.


#include "HitBoxComponent.h"
#include "../../Utilities/SvUtilities.h"

UHitBoxComponent::UHitBoxComponent(const FObjectInitializer& ObjectInitializer)
	: UBoxComponent(ObjectInitializer) {

	//SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionChannel(), ECR_Overlap); 
	SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap);
	SetCollisionObjectType(USvUtilities::GetBulletCollisionObjectChannel());

	DamageMultiplier = 1.0f;
}

FVector UHitBoxComponent::GetWorldLocation() {
	return this->GetComponentLocation();
}

void UHitBoxComponent::SetHitDimageMultiplier(float value) {
	DamageMultiplier = value;
}

float UHitBoxComponent::GetHitDamageMultiplier() {
	return DamageMultiplier;
}