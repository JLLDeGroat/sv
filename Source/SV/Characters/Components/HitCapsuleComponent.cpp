// Fill out your copyright notice in the Description page of Project Settings.


#include "HitCapsuleComponent.h"
#include "../../Utilities/SvUtilities.h"

UHitCapsuleComponent::UHitCapsuleComponent(const FObjectInitializer& ObjectInitializer) 
	: UCapsuleComponent(ObjectInitializer) {

	//SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap);
	SetCollisionObjectType(USvUtilities::GetBulletCollisionObjectChannel());

	SetCanEverAffectNavigation(false);
	DamageMultiplier = 1.0f;
}

FVector UHitCapsuleComponent::GetWorldLocation() {
	return this->GetComponentLocation();
}

void UHitCapsuleComponent::SetHitDimageMultiplier(float value) {
	DamageMultiplier = value;
}

float UHitCapsuleComponent::GetHitDamageMultiplier() {
	return DamageMultiplier;
}