// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackComponent.h"
#include "EquipmentComponent.h"
#include "TargetingComponent.h"
#include "GridMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Utilities/SvUtilities.h"
#include "../Anim/CharAnimInstance.h"
#include "../../Equipment/Equipment.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Interfaces/SvChar.h"
#include "../../Delegates/HudDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "AnimSpeedComponent.h"
#include "CharacterDetailsComponent.h"
#include "../../GameModes/Managers/CharacterManager.h"
#include "../../Runnables/PostMovementRunnable.h"
#include "../../Player/Actions/TargetAction.h"
#include "../../Player/Components/PawnCameraComponent.h"
#include "../../Player/Utility/TargetingIndicatorActor.h"
#include "../../GameModes/Managers/OverwatchManager.h"

UAttackComponent::UAttackComponent(const FObjectInitializer &ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentAttackState = EAttackState::CS_NONE;
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}
#pragma optimize("", off)
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
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
			AnimInstance->SetIsAttacking(true, CurrentAttackState == EAttackState::CS_RotatingToShoot ? EAttackType::AT_BasicFire : EAttackType::AT_BasicMelee);
		}
	}
	else if (CurrentAttackState == EAttackState::CS_MoveAttack)
	{
		auto lookAtRot = UGridUtilities::FindLookAtRotation(GetOwner()->GetActorLocation(), CurrentTargetLocation);
		lookAtRot.Pitch = GetOwner()->GetActorRotation().Pitch;
		lookAtRot.Roll = GetOwner()->GetActorRotation().Roll;
		lookAtRot += AdditionalRotation;

		auto newRotation = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), lookAtRot, DeltaTime, 100);
		GetOwner()->SetActorRotation(newRotation);

		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), MoveToLocation, DeltaTime, 150);
		DrawDebugLine(GetOwner()->GetWorld(), GetOwner()->GetActorLocation(), newLocation, FColor::Green, true, 10, 0, 10);
		GetOwner()->SetActorLocation(newLocation);

		if ((newRotation.Yaw - 2.5f) < lookAtRot.Yaw &&
			(newRotation.Yaw + 2.5f) > lookAtRot.Yaw &&
			FVector::Dist(MoveToLocation, GetOwner()->GetActorLocation()) < 5)
			SetComponentTickEnabled(false);
	}
	else if (CurrentAttackState == EAttackState::CS_Return)
	{
		auto newRotation = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), InitialRotation, DeltaTime, 100);
		GetOwner()->SetActorRotation(newRotation);

		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), InitialLocation, DeltaTime, 150);
		GetOwner()->SetActorLocation(newLocation);

		if ((newRotation.Yaw - 2.5f) < InitialRotation.Yaw &&
			(newRotation.Yaw + 2.5f) > InitialRotation.Yaw &&
			FVector::Dist(InitialLocation, GetOwner()->GetActorLocation()) < 5)
		{
			SetComponentTickEnabled(false);
			UDebugMessages::LogDisplay(this, "finished attacking");

			PostShootRunnable = NewObject<UPostMovementRunnable>()
									->InsertVariables(GetOwner())
									->Initialise(GetWorld())
									->Begin();
		}
	}
}

void UAttackComponent::UpdateCurrentAttackState(EAttackState attackState)
{
	CurrentAttackState = attackState;
	CurrentTargetCharacter = nullptr;
	SetComponentTickEnabled(true);
}

void UAttackComponent::TryAttackTarget(FVector sourceGridLocation, TScriptInterface<ISvChar> targetCharacter, bool bIsRange)
{
	CurrentTargetCharacter = targetCharacter;
	TryAttackLocation(sourceGridLocation, UGridUtilities::GetNormalisedGridLocation(targetCharacter->GetAsActor()->GetActorLocation()), 0, bIsRange);
}

void UAttackComponent::ReturnCharacterAnimationSpeedsToNormal()
{
	auto characterManager = USvUtilities::GetGameModeCharacterManager(GetOwner()->GetWorld());
	auto ownerDetails = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (characterManager && ownerDetails)
	{
		auto characterControl = ownerDetails->GetCharacterControl() == ECharacterControl::CC_AI ? ECharacterControl::CC_Player : ECharacterControl::CC_AI;

		TArray<TScriptInterface<ISvChar>> charactersToSlow;
		characterManager->GetCharacterListOfCharacterType(characterControl, charactersToSlow);

		for (int i = 0; i < charactersToSlow.Num(); i++)
		{
			if (charactersToSlow[i])
			{
				auto gridMovement = charactersToSlow[i]->GetAsActor()->GetComponentByClass<UGridMovementComponent>();
				if (gridMovement)
					gridMovement->ResetMovementAndAnimPlayBack();
			}
		}
	}
}

EOverwatchResult UAttackComponent::TryActivateOverwatch(AActor *targetActor, UPrimitiveComponent *componentWithinOverwatch)
{
	auto owner = GetOwner();
	auto controller = GetWorld()->GetFirstPlayerController();
	auto pawnCameraComponent = controller->GetPawn()->GetComponentByClass<UPawnCameraComponent>();

	auto targetComponent = owner->GetComponentByClass<UTargetingComponent>();

	if (!targetComponent)
	{
		UDebugMessages::LogError(this, "could not find target component");
		return EOverwatchResult::OR_Error;
	}

	targetComponent->DetermineTargetData();
	auto targetData = targetComponent->GetTargetDataForActor(targetActor);
	if (!targetData)
	{
		UDebugMessages::LogWarning(this, "could not find target data, TODO set retry for half a second");
		return EOverwatchResult::OR_Delay;
	}

	auto attackComponent = owner->GetComponentByClass<UAttackComponent>();
	auto equipmentComponent = owner->GetComponentByClass<UEquipmentComponent>();
	auto movementComponent = targetActor->GetComponentByClass<UGridMovementComponent>();
	if (!attackComponent || !equipmentComponent ||
		!equipmentComponent->GetPrimaryEquipment() ||
		!equipmentComponent->GetPrimaryEquipment()->GetComponentByClass<UEquipmentDetailsComponent>() || !movementComponent)
	{
		UDebugMessages::LogError(this, "could not get attack, equipment or movement component");
		return EOverwatchResult::OR_Error;
	}

	auto equipmentDetails = equipmentComponent->GetPrimaryEquipment()->GetComponentByClass<UEquipmentDetailsComponent>();

	movementComponent->SetMovementForOverwatchResponse();

	auto accuracy = USvUtilities::DetermineAccuracyInidicatorScale(owner->GetActorLocation(), targetActor->GetActorLocation(), equipmentDetails->GetAccuracy(),
																   equipmentDetails->GetAccuracyDecay(), equipmentDetails->GetBaseAccuracy());

	auto targetAction = controller->GetComponentByClass<UTargetAction>();
	if (!targetAction || !pawnCameraComponent)
	{
		UDebugMessages::LogError(this, "no targeting action or pawn camera compoment, failed");
		return EOverwatchResult::OR_Error;
	}

	auto overwatchManager = USvUtilities::GetGameModeOverwatchManager(GetOwner()->GetWorld());

	if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_OverwatchCinematicShoot ||
		overwatchManager->GetIsCurrentlyOverwatchAnimating())
	{
		UDebugMessages::LogWarning(this, "Already in Cinematic Overwatch or currently overwatch animating");
		return EOverwatchResult::OR_Delay;
	}
	else
	{
		pawnCameraComponent->DoOverwatchCinematicAttackCameraMovement(owner, targetActor);
		attackComponent->TryAttackLocation(targetData->GetShootLocation(), componentWithinOverwatch->GetComponentLocation(),
										   targetAction->SetScaleAndGetTargetingRadius(FVector(accuracy)));
		return EOverwatchResult::OR_Started;
	}
}

void UAttackComponent::TryAttackLocation(FVector sourceGridLocation, FVector location, float locationRadius, bool bIsRange)
{
	// assuming the gun is a AR
	// assuming we dont have to move
	CurrentTargetLocation = location;
	CurrentAttackRandomRadius = locationRadius;

	auto gridLocation = UGridUtilities::GetNormalisedGridLocation(GetOwner()->GetActorLocation());

	if (UGridUtilities::GetLocationsAtSameGrid(gridLocation, sourceGridLocation))
	{
		if (bIsRange)
		{
			InitialLocation = GetOwner()->GetActorLocation();
			InitialRotation = GetOwner()->GetActorRotation();
			CurrentAttackState = EAttackState::CS_RotatingToShoot;
		}
		else
		{
			InitialLocation = GetOwner()->GetActorLocation();
			InitialRotation = UGridUtilities::FindLookAtRotation(gridLocation, location);
			CurrentAttackState = EAttackState::CS_RotatingToMelee;
		}
		SetComponentTickEnabled(true);
	}
	else
	{
		// looking at the grid we move to in order to fire
		auto attack = DetermineAttackStateFromDirection(gridLocation, sourceGridLocation, location);

		DrawDebugLine(GetOwner()->GetWorld(), sourceGridLocation, location, FColor::Blue, false, 10, 0, 10);

		AnimInstance->SetIsAttacking(true, attack);
		CurrentAttackState = EAttackState::CS_MoveAttack;
		SetComponentTickEnabled(true);
	}

	auto characterManager = USvUtilities::GetGameModeCharacterManager(GetOwner()->GetWorld());
	auto ownerDetails = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (characterManager && ownerDetails)
	{
		auto characterControl = ownerDetails->GetCharacterControl() == ECharacterControl::CC_AI ? ECharacterControl::CC_Player : ECharacterControl::CC_AI;

		TArray<TScriptInterface<ISvChar>> charactersToSlow;
		characterManager->GetCharacterListOfCharacterType(characterControl, charactersToSlow);

		for (int i = 0; i < charactersToSlow.Num(); i++)
		{
			if (charactersToSlow[i])
			{
				auto animSpeed = charactersToSlow[i]->GetAsActor()->GetComponentByClass<UAnimSpeedComponent>();
				if (animSpeed)
					animSpeed->SlowAnimation();
			}
		}
	}
	AnimInstance->SetIsCrouching(false);
}

FVector UAttackComponent::GetCurrentTargetLocation() const
{
	return CurrentTargetLocation;
}

EAttackType UAttackComponent::DetermineAttackStateFromDirection(FVector currentGridLoc, FVector movementLoc, FVector targetLoc)
{
	FRotator movementAtRot = UGridUtilities::FindLookAtRotation(currentGridLoc, movementLoc);
	FRotator targetLookAtRot = UGridUtilities::FindLookAtRotation(currentGridLoc, targetLoc);

	// finding out if target is to the left or right of move to rotation
	UDebugMessages::LogDisplay(this, movementAtRot.ToString());
	UDebugMessages::LogDisplay(this, movementAtRot.Vector().ToString());

	UDebugMessages::LogDisplay(this, targetLookAtRot.ToString());
	UDebugMessages::LogDisplay(this, targetLookAtRot.Vector().ToString());

	InitialLocation = currentGridLoc;
	InitialRotation = movementAtRot;
	MoveToLocation = movementLoc;

	auto movemantAtRotAsVector = movementAtRot.Vector();
	auto targetLookRotAsVector = targetLookAtRot.Vector();

	const float PrecisionThreshold = 1e-6f; 

	auto forwardVec = GetOwner()->GetActorForwardVector();

	if (movementAtRot.Vector().X >= PrecisionThreshold && movementAtRot.Vector().X <= 1)
	{
		// its moving from x: 100 to x: 200 (for example)
		if (targetLookAtRot.Vector().Y >= PrecisionThreshold)
		{
			AdditionalRotation = FRotator(0, -90, 0);
			return EAttackType::AT_MoveAndFire_Right;
		}
		else
		{
			AdditionalRotation = FRotator(0, 90, 0);
			return EAttackType::AT_MoveAndFire_Left;
		}
	}
	else if (movementAtRot.Vector().Y >= PrecisionThreshold && movementAtRot.Vector().Y <= 1)
	{
		// its moving from Y: 100 to Y: 200 (for example)
		if (targetLookAtRot.Vector().X >= 0)
		{
			AdditionalRotation = FRotator(0, 90, 0);
			return EAttackType::AT_MoveAndFire_Left;
		}
		else
		{
			AdditionalRotation = FRotator(0, -90, 0);
			return EAttackType::AT_MoveAndFire_Right;
		}
	}
	else if (movementAtRot.Vector().X < 0 && movementAtRot.Vector().X >= -1)
	{
		// its moving from X: 200 to X: 100 (for example)
		if (targetLookAtRot.Vector().Y >= 0)
		{
			AdditionalRotation = FRotator(0, 90, 0);
			return EAttackType::AT_MoveAndFire_Left;
		}
		else
		{
			AdditionalRotation = FRotator(0, -90, 0);
			return EAttackType::AT_MoveAndFire_Right;
		}
	}
	else
	{
		if (targetLookAtRot.Vector().X >= 0)
		{
			AdditionalRotation = FRotator(0, -90, 0);
			return EAttackType::AT_MoveAndFire_Right;
		}
		else
		{
			AdditionalRotation = FRotator(0, 90, 0);
			return EAttackType::AT_MoveAndFire_Left;
		}
	}

	// if (movementAtRot.Vector().Y >= 0)
	// {
	// 	if (targetLookAtRot.Vector().X < 0)
	// 	{
	// 		AdditionalRotation = FRotator(0, 90, 0);
	// 		return EAttackType::AT_MoveAndFire_Left;
	// 	}
	// 	else
	// 	{
	// 		AdditionalRotation = FRotator(0, -90, 0);
	// 		return EAttackType::AT_MoveAndFire_Right;
	// 	}
	// }
	// else
	// {
	// 	if (targetLookAtRot.Vector().X > 0)
	// 	{
	// 		AdditionalRotation = FRotator(0, -90, 0);
	// 		return EAttackType::AT_MoveAndFire_Right;
	// 	}
	// 	else
	// 	{
	// 		AdditionalRotation = FRotator(0, 90, 0);
	// 		return EAttackType::AT_MoveAndFire_Left;
	// 	}
	// }
}

TScriptInterface<ISvChar> UAttackComponent::GetCurrentTargetCharacter()
{
	return CurrentTargetCharacter;
}

float UAttackComponent::GetCurrentAttackRandomRadius() const
{
	return CurrentAttackRandomRadius;
}
#pragma optimize("", on)