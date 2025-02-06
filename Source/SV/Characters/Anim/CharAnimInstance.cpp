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
#include "../Components/DropResourceComponent.h"
#include "../Components/CharacterDetailsComponent.h"
#include "../Components/PickupResourceComponent.h"
#include "../Components/SpawnInComponent.h"
#include "../Components/HealthKitsComponent.h"
#include "../Components/AIComponent.h"
#include "../Components/SuicideComponent.h"
#include "../Components/ClimbLadderComponent.h"
#include "../../Player/Components/PawnCameraComponent.h"
#include "../../Player/Components/CameraOverlapComponent.h"
#include "../../Player/GamePlayerController.h"
#include "../../Player/Actions/GrenadeActionComponent.h"
#include "../../Equipment/Equipment.h"
#include "../../Equipment/Throwable/Components/ThrowTravelComponent.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../Components/AIComponent.h"
#include "Animation/AnimNode_StateMachine.h"
#include "../../Runnables/PostMovementRunnable.h"
#include "NiagaraComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../../GameModes/Managers/OverwatchManager.h"

UCharAnimInstance::UCharAnimInstance(const FObjectInitializer& ObjectInitializer)
	: UAnimInstance(ObjectInitializer) {
	bUseMultiThreadedAnimationUpdate = true;
	Speed = 0;
	bIsAttacking = false;
	bIsClimbing = false;
	AnimPlayRate = 1.0f;
	CharacterAnimState = ECharacterAnimState::AS_AR;
}

void UCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	//getting state machines, future code helper
	//auto stateMachine = GetStateMachineInstanceFromName(FName("GruntStateMachine"));
	//if (stateMachine) {
	//	auto currentState = stateMachine->GetCurrentState();
	//	auto stateName = stateMachine->GetCurrentStateName();

	//	UDebugMessages::LogDisplay(this, "current state " + stateName.ToString());
	//}
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
void UCharAnimInstance::SetIsReloading(bool val) {
	bIsReloading = val;
}
void UCharAnimInstance::SetIsTakenDamage(bool val) {
	bHasTakenDamage = val;
}
void UCharAnimInstance::SetIsAiActive(bool val) {
	bIsAiActive = val;
}
void UCharAnimInstance::SetIsPickingUp(bool val) {
	bIsPickingUp = val;
}
void UCharAnimInstance::SetIsSpawningFromGround(bool val) {
	bSpawningFromGround = val;
}
void UCharAnimInstance::SetCharacterAnimState(ECharacterAnimState animState) {
	CharacterAnimState = animState;
}
void UCharAnimInstance::SetIsHealingSelf(bool val) {
	bIsHealingSelf = val;
}
void UCharAnimInstance::SetIsHealingAlly(bool val) {
	bIsHealingAlly = val;
}
void UCharAnimInstance::SetIsSuiciding(bool val) {
	bIsSuiciding = val;
}
void UCharAnimInstance::SetIsClimbing(bool val, EClimbType climbType) {
	ClimbType = climbType;
	bIsClimbing = val;
}

void UCharAnimInstance::OnGunPreFireActivate() {
	auto owningActor = GetOwningActor();
	auto equipmentComponent = owningActor->GetComponentByClass<UEquipmentComponent>();
	if (equipmentComponent)
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([equipmentComponent]
			{
				//TODO assuming its a gun
				equipmentComponent->TryActivateMainEquipment();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
}
void UCharAnimInstance::OnGunPostFireDeactivate() {
	auto owningActor = GetOwningActor();
	auto equipmentComponent = owningActor->GetComponentByClass<UEquipmentComponent>();
	if (equipmentComponent)
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([equipmentComponent]
			{
				//TODO assuming its a gun
				equipmentComponent->TryDeactivateMainEquipment();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
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

	if (owningActor) {
		bIsAttacking = false;
		auto attackComponent = owningActor->GetComponentByClass<UAttackComponent>();

		auto playerController = owningActor->GetWorld()->GetFirstPlayerController<AGamePlayerController>();
		auto pawn = playerController->GetPawn();
		auto cameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();
		auto cameraOverlapComponent = pawn->GetComponentByClass<UCameraOverlapComponent>();
		auto currentAttackType = AttackType;

		bool isOverwatchAttack = cameraComponent->GetCurrentCameraState() == ECameraState::CS_OverwatchCinematicShoot;

		if (currentAttackType == EAttackType::AT_MoveAndFire_Right ||
			currentAttackType == EAttackType::AT_MoveAndFire_Left ||
			currentAttackType == EAttackType::AT_BasicFire &&
			(attackComponent && cameraComponent && cameraOverlapComponent))
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([attackComponent, isOverwatchAttack]
				{
					if (!isOverwatchAttack)
						attackComponent->ReturnCharacterAnimationSpeedsToNormal();

					attackComponent->UpdateCurrentAttackState(EAttackState::CS_Return);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);

		auto hasAiComponent = owningActor->GetComponentByClass<UAIComponent>();
		if (!hasAiComponent)
			GetWorld()->GetTimerManager().SetTimer(OnFinishFireHandle, this, &UCharAnimInstance::OnFinishFire_PostDelay, 1.0f, false);
	}
}

void UCharAnimInstance::OnFinishFire_PostDelay() {
	auto owningActor = GetOwningActor();
	if (owningActor) {
		auto playerController = owningActor->GetWorld()->GetFirstPlayerController<AGamePlayerController>();
		auto pawn = playerController->GetPawn();
		auto cameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();
		auto cameraOverlapComponent = pawn->GetComponentByClass<UCameraOverlapComponent>();

		bool wasOverwatch = cameraComponent->GetCurrentCameraState() == ECameraState::CS_OverwatchCinematicShoot;

		if (cameraComponent && cameraOverlapComponent && playerController)
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([cameraComponent, cameraOverlapComponent, playerController, wasOverwatch]
				{
					cameraComponent->UpdateCameraState(ECameraState::CS_Control);
					cameraOverlapComponent->ResetOverlapComponent();
					playerController->SetMouseAsUi();
				},
				TStatId(), nullptr, ENamedThreads::GameThread);

		if (wasOverwatch) {
			auto overwatchManager = USvUtilities::GetGameModeOverwatchManager(owningActor->GetWorld());

			if (!overwatchManager->GetIsCurrentlyOverwatchAnimating()) {
				return UDebugMessages::LogError(this, "tried to end overwatch animation, but its not currently overwatching");
			}

			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([overwatchManager]
				{
					overwatchManager->OnOverwatchItemComplete();
				},
				TStatId(), nullptr, ENamedThreads::GameThread);

		}
	}
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
	auto owningActor = GetOwningActor();
	auto attackComponent = owningActor->GetComponentByClass<UAttackComponent>();

	if (!attackComponent)
		return UDebugMessages::LogError(this, "finished melee without an attack component");

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([attackComponent]
		{
			attackComponent->UpdateCurrentAttackState(EAttackState::CS_Return);
		},
		TStatId(), nullptr, ENamedThreads::GameThread);
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
			equipmentComponent->AttachEquipmentToSocket(EAttachType::AT_Backpack, primaryEquipment);
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
			equipmentComponent->AttachEquipmentToSocket(EAttachType::AT_RightHand, primaryEquipment);
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

void UCharAnimInstance::OnReloadFinish() {
	bIsReloading = false;

	UDebugMessages::LogDisplay(this, "finishing reloading");

	auto owningActor = GetOwningActor();

	auto equipmentComponent = owningActor->GetComponentByClass<UEquipmentComponent>();
	if (!equipmentComponent)
		return UDebugMessages::LogError(this, "failed to get equipment component");

	auto primaryEquipment = equipmentComponent->GetPrimaryEquipment();
	if (!primaryEquipment)
		return UDebugMessages::LogError(this, "failed to get primaryEquipment");

	auto equipmentDetailsComponent = primaryEquipment->GetComponentByClass<UEquipmentDetailsComponent>();
	equipmentDetailsComponent->FillRounds();

	auto characterDetails = owningActor->GetComponentByClass<UCharacterDetailsComponent>();

	if (!characterDetails)
		return UDebugMessages::LogError(this, "failed to get character details component");

	characterDetails->RemoveActionPoints(equipmentDetailsComponent->GetReloadApCost());

	BaseRunnable = NewObject<UPostMovementRunnable>()
		->InsertVariables(owningActor)
		->Initialise(owningActor->GetWorld())
		->Begin();
}
void UCharAnimInstance::OnFinishTakenDamage() {
	bHasTakenDamage = false;
}
void UCharAnimInstance::OnInitializePickup() {
	bIsPickingUp = false;

	auto owningActor = GetOwningActor();

	if (owningActor) {
		auto resourcePickup = owningActor->GetComponentByClass<UPickupResourceComponent>();

		if (!resourcePickup)
			return UDebugMessages::LogError(this, "failed to get resource pickup component");

		resourcePickup->AssignPickup();
	}
}
void UCharAnimInstance::OnFinishPickup() {
	auto owningActor = GetOwningActor();

	if (owningActor)
		BaseRunnable = NewObject<UPostMovementRunnable>()
		->InsertVariables(owningActor)
		->Initialise(owningActor->GetWorld())
		->Begin();
}

void UCharAnimInstance::SpawningFromGroundStartMovingUp() {
	auto owningActor = GetOwningActor();
	if (owningActor && owningActor->GetComponentByClass<USpawnInComponent>()) {
		auto component = owningActor->GetComponentByClass<USpawnInComponent>();
		component->SetComponentTickEnabled(true);
	}
}
void UCharAnimInstance::SpawningFromGroundFinishMovingUp() {
	UDebugMessages::LogError(this, "this one currently does nothing");
}
void UCharAnimInstance::SpawningFromGroundAlterMovingUpSpeed(float speed) {
	auto owningActor = GetOwningActor();
	if (owningActor && owningActor->GetComponentByClass<USpawnInComponent>()) {
		auto component = owningActor->GetComponentByClass<USpawnInComponent>();
		component->UpdateMovementSpeed(speed);
	}
}


void UCharAnimInstance::OnHolsterCurrentWeapon() {
	auto owningActor = GetOwningActor();
	if (owningActor && owningActor->GetComponentByClass<UEquipmentComponent>()) {
		auto equipmentComponent = owningActor->GetComponentByClass<UEquipmentComponent>();
		equipmentComponent->HolsterCurrentMainEquipment();
	}
}
void UCharAnimInstance::OnUnHolsterNewWeapon() {
	auto owningActor = GetOwningActor();
	if (owningActor && owningActor->GetComponentByClass<UEquipmentComponent>()) {
		auto equipmentComponent = owningActor->GetComponentByClass<UEquipmentComponent>();
		equipmentComponent->UnholsterNewMainEquipment();
	}
}

void UCharAnimInstance::OnFinishWeaponSwapping() {
	auto owningActor = GetOwningActor();
	if (owningActor && owningActor->GetComponentByClass<UEquipmentComponent>()) {

		auto equipmentComponent = owningActor->GetComponentByClass<UEquipmentComponent>();

		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([equipmentComponent]
			{
				equipmentComponent->FinishSwapWeapon();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UCharAnimInstance::OnHealingComplete() {
	auto owningActor = GetOwningActor();
	if (owningActor && owningActor->GetComponentByClass<UHealthKitsComponent>()) {
		auto healthKitComponent = owningActor->GetComponentByClass<UHealthKitsComponent>();
		healthKitComponent->UseActiveHealthKitOnActor();
	}
}
void UCharAnimInstance::OnSpawnHealthKit() {
	auto owningActor = GetOwningActor();
	if (owningActor && owningActor->GetComponentByClass<UHealthKitsComponent>()) {
		auto healthKitComponent = owningActor->GetComponentByClass<UHealthKitsComponent>();
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([healthKitComponent]
			{
				healthKitComponent->SpawnHealthKitOnHand();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
void UCharAnimInstance::OnDespawnHealthKit() {
	auto owningActor = GetOwningActor();
	if (owningActor && owningActor->GetComponentByClass<UHealthKitsComponent>()) {
		auto healthKitComponent = owningActor->GetComponentByClass<UHealthKitsComponent>();
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([healthKitComponent]
			{
				healthKitComponent->DespawnHealthKitOnHand();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UCharAnimInstance::SetHealthKitActivation(bool val) {
	auto owningActor = GetOwningActor();
	if (owningActor && owningActor->GetComponentByClass<UHealthKitsComponent>()) {
		auto healthKitcomponent = owningActor->GetComponentByClass<UHealthKitsComponent>();
		auto healthKit = healthKitcomponent->GetHealthKitActor();

		AnimPlayRate = val ? .4f : 1.0f;

		if (!healthKit)
			return UDebugMessages::LogError(this, "failed to get healthkit actor");

		auto niagaraComp = healthKit->GetComponentByClass<UNiagaraComponent>();
		if (niagaraComp) {
			if (val) niagaraComp->Activate();
			else niagaraComp->Deactivate();
		}
	}
}

void UCharAnimInstance::OnSuicideExplosion() {
	auto owningActor = GetOwningActor();
	if (owningActor) {
		auto suicideComponent = owningActor->GetComponentByClass<USuicideComponent>();

		if (suicideComponent) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([suicideComponent]
				{
					suicideComponent->ActivateSuicide();
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}


void UCharAnimInstance::OnClimbGoUp() {
	auto owningActor = GetOwningActor();
	if (owningActor) {
		auto ladderComponent = owningActor->GetComponentByClass<UClimbLadderComponent>();
		if (ladderComponent) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([ladderComponent]
				{
					ladderComponent->SetMoveUp(true);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}
void UCharAnimInstance::OnClimbPauseGoUp() {
	auto owningActor = GetOwningActor();
	if (owningActor) {
		auto ladderComponent = owningActor->GetComponentByClass<UClimbLadderComponent>();
		if (ladderComponent) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([ladderComponent]
				{
					ladderComponent->SetMoveUp(false);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}

void UCharAnimInstance::OnClimbFinish() {
	auto owningActor = GetOwningActor();
	if (owningActor) {
		auto ladderComponent = owningActor->GetComponentByClass<UClimbLadderComponent>();
		if (ladderComponent) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([ladderComponent]
				{
					ladderComponent->SetMoveUp(false);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}

void UCharAnimInstance::OnPostClimbStart() {
	auto owningActor = GetOwningActor();
	if (owningActor) {
		auto ladderComponent = owningActor->GetComponentByClass<UClimbLadderComponent>();
		if (ladderComponent) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([ladderComponent]
				{
					ladderComponent->SetPostClimb(true);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}

void UCharAnimInstance::OnClimbPreFall() {
	auto owningActor = GetOwningActor();
	if (owningActor) {
		auto ladderComponent = owningActor->GetComponentByClass<UClimbLadderComponent>();
		if (ladderComponent) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([ladderComponent]
				{
					ladderComponent->SetPreFallMove(true);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}
void UCharAnimInstance::OnClimbFallStart() {
	auto owningActor = GetOwningActor();
	if (owningActor) {
		auto ladderComponent = owningActor->GetComponentByClass<UClimbLadderComponent>();
		if (ladderComponent) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([ladderComponent]
				{
					ladderComponent->SetPreFallMove(false);
					ladderComponent->SetFallMove(true);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}
void UCharAnimInstance::OnClimbFallStop() {
	auto owningActor = GetOwningActor();
	if (owningActor) {
		auto ladderComponent = owningActor->GetComponentByClass<UClimbLadderComponent>();
		if (ladderComponent) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([ladderComponent]
				{
					ladderComponent->SetFallMove(false);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}

bool UCharAnimInstance::GetIsHealing() {
	return bIsHealingAlly || bIsHealingSelf;
}