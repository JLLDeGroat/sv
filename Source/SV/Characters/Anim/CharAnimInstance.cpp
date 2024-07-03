// Fill out your copyright notice in the Description page of Project Settings.


#include "CharAnimInstance.h"
#include "../BaseCharacter.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Components/EquipmentComponent.h"
#include "../Components/SkillsComponent.h"
#include "../Components/AttackComponent.h"
#include "../Components/ThrowableComponent.h"
#include "../Components/VaultObstacleComponent.h"
#include "../Components/GridMovementComponent.h"
#include "../../Player/Components/PawnCameraComponent.h"
#include "../../Player/Components/CameraOverlapComponent.h"
#include "../../Player/GamePlayerController.h"
#include "../../Player/Actions/GrenadeActionComponent.h"
#include "../../Equipment/Equipment.h"
#include "../../Equipment/Throwable/Components/ThrowTravelComponent.h"

UCharAnimInstance::UCharAnimInstance(const FObjectInitializer& ObjectInitializer)
	: UAnimInstance(ObjectInitializer) {
	bUseMultiThreadedAnimationUpdate = true;
	Speed = 0;
	bIsAttacking = false;
	AnimPlayRate = 1.0f;
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
void UCharAnimInstance::SetIsThrowing(bool value, EAttackType attackType) {
	AttackType = attackType;
	bIsThrowing = value;
}
void UCharAnimInstance::SetIsVaulting(bool val) {
	bIsVaulting = val;
}
void UCharAnimInstance::SetIsCrouching(bool val) {
	bIsCrouching = val;
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

	auto playerController = owningActor->GetWorld()->GetFirstPlayerController<AGamePlayerController>();
	auto pawn = playerController->GetPawn();
	auto cameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();
	auto cameraOverlapComponent = pawn->GetComponentByClass<UCameraOverlapComponent>();

	bIsAttacking = false;
	auto currentAttackType = AttackType;

	if (currentAttackType == EAttackType::AT_MoveAndFire_Right ||
		currentAttackType == EAttackType::AT_MoveAndFire_Left  ||
		currentAttackType == EAttackType::AT_BasicFire &&
		(attackComponent && cameraComponent && cameraOverlapComponent))
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([attackComponent, cameraComponent, playerController, cameraOverlapComponent]
			{
				attackComponent->ReturnCharacterAnimationSpeedsToNormal();
				attackComponent->UpdateCurrentAttackState(EAttackState::CS_Return);
				cameraComponent->UpdateCameraState(ECameraState::CS_Control);
				playerController->SetMouseAsUi();
				cameraOverlapComponent->ResetOverlapComponent();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
}

void UCharAnimInstance::OnMeleeHit() {
	auto owningActor = GetOwningActor();
	auto skillComponent = owningActor->GetComponentByClass<USkillsComponent>();
	auto equipmentComponent = owningActor->GetComponentByClass<UEquipmentComponent>();

	if (!skillComponent || !equipmentComponent)
		return UDebugMessages::LogError(this, "there was no skill/equipment component, will not complete on melee hit");

	if (skillComponent->GetCurrentActiveSkill()) {
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([skillComponent]
			{ skillComponent->UseCurrentSkillOnTarget(); },
			TStatId(), nullptr, ENamedThreads::GameThread);
	}
	else {

	}
}
void UCharAnimInstance::OnFinishMelee() {
	bIsAttacking = false;

	/*FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([]
		{

		},
		TStatId(), nullptr, ENamedThreads::GameThread);*/
}

void UCharAnimInstance::OnFinishVault() {
	bIsVaulting = false;

	auto owningActor = GetOwningActor();
	auto movementComponent = owningActor->GetComponentByClass<UGridMovementComponent>();
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([movementComponent]
		{
			movementComponent->ResetMovementSpeed();
		},
		TStatId(), nullptr, ENamedThreads::GameThread);
}

void UCharAnimInstance::UpdateAnimPlayRate(float newRate) {
	AnimPlayRate = newRate;
	UDebugMessages::LogDisplay(this, "altered anim play rate to  " + FString::SanitizeFloat(newRate));
}

void UCharAnimInstance::OnHolsterWeapon() {
	auto owningActor = GetOwningActor();

	auto equipmentComponent = owningActor->GetComponentByClass<UEquipmentComponent>();

	if (!equipmentComponent) 
		return UDebugMessages::LogError(this, "failed to get equipmentcomponent");

	auto primaryEquipment = equipmentComponent->GetPrimaryEquipment();

	if (!primaryEquipment)
		return UDebugMessages::LogError(this, "failed to get primary equipment");


	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([equipmentComponent, primaryEquipment]
		{
			equipmentComponent->AttachEquipmentToSocket(EAttachType::AT_Backpack, primaryEquipment, "BackPackSocket");
		},
		TStatId(), nullptr, ENamedThreads::GameThread);
}
void UCharAnimInstance::OnUnholsteredWeapon() {
	auto owningActor = GetOwningActor();

	auto equipmentComponent = owningActor->GetComponentByClass<UEquipmentComponent>();
	if (!equipmentComponent) return UDebugMessages::LogError(this, "failed to get equipmentcomponent");

	auto primaryEquipment = equipmentComponent->GetPrimaryEquipment();
	if (!primaryEquipment) return UDebugMessages::LogError(this, "failed to get primary equipment");


	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([equipmentComponent, primaryEquipment]
		{
			equipmentComponent->AttachEquipmentToSocket(EAttachType::AT_RightHand, primaryEquipment, "RightHandSocket");
		},
		TStatId(), nullptr, ENamedThreads::GameThread);
}
void UCharAnimInstance::OnGrabbedThrowable() {
	auto owningActor = GetOwningActor();
	auto throwableComponent = owningActor->GetComponentByClass<UThrowableComponent>();

	auto controller = owningActor->GetWorld()->GetFirstPlayerController<AGamePlayerController>();

	
	if (!controller || !controller->GetComponentByClass<UGrenadeActionComponent>() || !throwableComponent)
		return UDebugMessages::LogError(this, "failed to get controller or no grenade action component, or no throwable component found");

	auto grenadeAction = controller->GetComponentByClass<UGrenadeActionComponent>();
	auto grenadeThrowable = grenadeAction->GetThrowableChosen();
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([throwableComponent, grenadeThrowable]
		{
			throwableComponent->SpawnThrowableOfTypeAtRightHand(grenadeThrowable);
		},
		TStatId(), nullptr, ENamedThreads::GameThread);
}
void UCharAnimInstance::OnReleasedThrowable() {
	auto owningActor = GetOwningActor();
	auto throwableComponent = owningActor->GetComponentByClass<UThrowableComponent>();
	auto throwable = throwableComponent->GetThrownEquipment();

	if (!throwable || !throwable->GetComponentByClass<UThrowTravelComponent>())
		return UDebugMessages::LogError(this, "failed to get current throwable or throw travel component");
	auto travelComponent = throwable->GetComponentByClass<UThrowTravelComponent>();

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([travelComponent]
		{
			travelComponent->BeginTravel();
		},
		TStatId(), nullptr, ENamedThreads::GameThread);
}