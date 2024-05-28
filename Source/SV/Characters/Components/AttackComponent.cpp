// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
#include "EquipmentComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Utilities/SvUtilities.h"
#include "../Anim/CharAnimInstance.h"
#include "../../Interfaces/SvChar.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UAttackComponent::UAttackComponent(const FObjectInitializer& ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer) 
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentAttackState = EAttackState::CS_NONE;
}

void UAttackComponent::BeginPlay() {
	Super::BeginPlay();
	SetComponentTickInterval(false);
}

void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentAttackState == EAttackState::CS_RotatingToMelee ||
		CurrentAttackState == EAttackState::CS_RotatingToShoot)
	{
		auto lookAtRot = UGridUtilities::FindLookAtRotation(GetOwner()->GetActorLocation(), CurrentTargetLocation);
		lookAtRot.Pitch = GetOwner()->GetActorRotation().Pitch;
		lookAtRot.Roll = GetOwner()->GetActorRotation().Roll;

		auto newRotation = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), lookAtRot, DeltaTime, 100);
		GetOwner()->SetActorRotation(newRotation);

		if ((newRotation.Yaw - 2.5f) < lookAtRot.Yaw &&
			(newRotation.Yaw + 2.5f) > lookAtRot.Yaw) 
		{
			UDebugMessages::LogDisplay(this, "starting the attack");
			SetComponentTickEnabled(false);
			AnimInstance->SetIsAttacking(true, CurrentAttackState == EAttackState::CS_RotatingToShoot ? 
				EAttackType::AT_BasicFire : 
				EAttackType::AT_BasicMelee);
		}
	}
	else if (CurrentAttackState == EAttackState::CS_MoveAttack) {
		auto lookAtRot = UGridUtilities::FindLookAtRotation(GetOwner()->GetActorLocation(), CurrentTargetLocation);
		lookAtRot.Pitch = GetOwner()->GetActorRotation().Pitch;
		lookAtRot.Roll = GetOwner()->GetActorRotation().Roll;
		lookAtRot += AdditionalRotation;

		auto newRotation = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), lookAtRot, DeltaTime, 100);
		GetOwner()->SetActorRotation(newRotation);

		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), MoveToLocation, DeltaTime, 150);
		GetOwner()->SetActorLocation(newLocation);

		if ((newRotation.Yaw - 2.5f) < lookAtRot.Yaw &&
			(newRotation.Yaw + 2.5f) > lookAtRot.Yaw &&
			FVector::Dist(MoveToLocation, GetOwner()->GetActorLocation()) < 5)
			SetComponentTickEnabled(false);
	}
	else if (CurrentAttackState == EAttackState::CS_Return) {
		auto newRotation = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), InitialRotation, DeltaTime, 100);
		GetOwner()->SetActorRotation(newRotation);

		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), InitialLocation, DeltaTime, 150);
		GetOwner()->SetActorLocation(newLocation);

		if ((newRotation.Yaw - 2.5f) < InitialRotation.Yaw &&
			(newRotation.Yaw + 2.5f) > InitialRotation.Yaw &&
			FVector::Dist(InitialLocation, GetOwner()->GetActorLocation()) < 5)
			SetComponentTickEnabled(false);
	}
}

void UAttackComponent::UpdateCurrentAttackState(EAttackState attackState) {
	CurrentAttackState = attackState;
	SetComponentTickEnabled(true);
}

void UAttackComponent::TryAttackTarget(FVector sourceGridLocation, TScriptInterface<ISvChar> targetCharacter, bool bIsRange) {
	CurrentTargetCharacter = targetCharacter;
	TryAttackLocation(sourceGridLocation, UGridUtilities::GetNormalisedGridLocation(targetCharacter->GetAsActor()->GetActorLocation()), bIsRange);
}

void UAttackComponent::TryAttackLocation(FVector sourceGridLocation, FVector location, bool bIsRange) {
	//assuming the gun is a AR
	//assuming we dont have to move
	CurrentTargetLocation = location;

	auto gridLocation = UGridUtilities::GetNormalisedGridLocation(GetOwner()->GetActorLocation());

	if (gridLocation == sourceGridLocation) {
		CurrentAttackState = bIsRange ? EAttackState::CS_RotatingToShoot : EAttackState::CS_RotatingToMelee;
		SetComponentTickEnabled(true);
	}
	else {
		//looking at the grid we move to in order to fire
		auto attack = DetermineAttackStateFromDirection(gridLocation, sourceGridLocation, location);
		AnimInstance->SetIsAttacking(true, attack);
		CurrentAttackState = EAttackState::CS_MoveAttack;
		SetComponentTickEnabled(true);
	}
}

FVector UAttackComponent::GetCurrentTargetLocation() const {
	return CurrentTargetLocation;
}

EAttackType UAttackComponent::DetermineAttackStateFromDirection(FVector currentGridLoc, FVector movementLoc, FVector targetLoc) {
	FRotator movementAtRot = UGridUtilities::FindLookAtRotation(currentGridLoc, movementLoc);
	FRotator targetLookAtRot = UGridUtilities::FindLookAtRotation(currentGridLoc, targetLoc);

	//finding out if target is to the left or right of move to rotation
	UDebugMessages::LogDisplay(this, movementAtRot.ToString());
	UDebugMessages::LogDisplay(this, movementAtRot.Vector().ToString());

	UDebugMessages::LogDisplay(this, targetLookAtRot.ToString());
	UDebugMessages::LogDisplay(this, targetLookAtRot.Vector().ToString());

	InitialLocation = currentGridLoc;
	InitialRotation = movementAtRot;
	MoveToLocation = movementLoc;

	if (movementAtRot.Vector().Y > 0) {
		if (targetLookAtRot.Vector().X > 0) {
			AdditionalRotation = FRotator(0, 90, 0);
			return EAttackType::AT_MoveAndFire_Left;
		}
		else {
			AdditionalRotation = FRotator(0, -90, 0);
			return EAttackType::AT_MoveAndFire_Right;
		}
	}
	else {
		if (targetLookAtRot.Vector().X > 0) {
			AdditionalRotation = FRotator(0, -90, 0);
			return EAttackType::AT_MoveAndFire_Right;
		}
		else {
			AdditionalRotation = FRotator(0, 90, 0);
			return EAttackType::AT_MoveAndFire_Left;
		}
	}
}

TScriptInterface<ISvChar> UAttackComponent::GetCurrentTargetCharacter() {
	return CurrentTargetCharacter;
}