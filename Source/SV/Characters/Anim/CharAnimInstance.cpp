// Fill out your copyright notice in the Description page of Project Settings.


#include "CharAnimInstance.h"
#include "../BaseCharacter.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Components/EquipmentComponent.h"
#include "../Components/AttackComponent.h"

UCharAnimInstance::UCharAnimInstance(const FObjectInitializer& ObjectInitializer)
	: UAnimInstance(ObjectInitializer) {
	bUseMultiThreadedAnimationUpdate = true;
	Speed = 0;
	bIsAttacking = false;
}

void UCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCharAnimInstance::UpdateSpeed(float value) {
	Speed = value;
}
void UCharAnimInstance::SetIsAttacking(bool value, EAttackType attackType) {
	AttackType = attackType;
	bIsAttacking = value;
}

void UCharAnimInstance::OnGunFire() {
	auto owningActor = GetOwningActor();
	auto equipmentComponent = owningActor->GetComponentByClass<UEquipmentComponent>();
	if (equipmentComponent)
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([equipmentComponent]
			{
				//TODO assuming its a gun
				equipmentComponent->FireEquippedGun();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
}


void UCharAnimInstance::OnFinishFire() {
	auto owningActor = GetOwningActor();
	auto attackComponent = owningActor->GetComponentByClass<UAttackComponent>();

	bIsAttacking = false;
	auto currentAttackType = AttackType;

	if (currentAttackType == EAttackType::AT_MoveAndFire_Right ||
		currentAttackType == EAttackType::AT_MoveAndFire_Left &&
		attackComponent)
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([attackComponent]
			{
				attackComponent->UpdateCurrentAttackState(EAttackState::CS_Return);
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
}