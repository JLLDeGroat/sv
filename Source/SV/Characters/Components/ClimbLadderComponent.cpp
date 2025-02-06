// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbLadderComponent.h"
#include "../Anim/CharAnimInstance.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Utilities/SvUtilities.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GridMovementComponent.h"

UClimbLadderComponent::UClimbLadderComponent(const FObjectInitializer& ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer) {

	PrimaryComponentTick.bCanEverTick = true;
	MoveUp = false;
}

void UClimbLadderComponent::BeginPlay() {
	Super::BeginPlay();

	SetComponentTickEnabled(false);
}

void UClimbLadderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentClimbType == EClimbType::CT_Up)
		ClimbUpTick(DeltaTime);
	else if (CurrentClimbType == EClimbType::CT_Down)
		ClimbDownTick(DeltaTime);

}

void UClimbLadderComponent::StartClimbingLadder(EMovementType movementType, FVector climbLocation, EClimbType climbType) {
	CurrentClimbType = climbType;

	AnimInstance->SetIsClimbing(true, climbType);
	PostClimbLocation = climbLocation;

	auto lookAtLadderRot = UGridUtilities::FindLookAtRotation(GetOwner()->GetActorLocation(), PostClimbLocation);
	lookAtLadderRot.Pitch = GetOwner()->GetActorRotation().Pitch;
	lookAtLadderRot.Roll = GetOwner()->GetActorRotation().Roll;
	GetOwner()->SetActorRotation(lookAtLadderRot);

	auto capsule = GetOwner()->GetComponentByClass<UCapsuleComponent>();
	capsule->SetEnableGravity(false);

	auto characterMovement = GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
	characterMovement->GravityScale = 0;

	PreClimbLocation = GetOwner()->GetActorLocation() - (GetOwner()->GetActorForwardVector() * 20);
	PreClimbing = true;

	GetOwner()->SetActorEnableCollision(false);

	/*capsule->SetSimulatePhysics(false);

	auto allComps = GetOwner()->GetComponents();
	for (UActorComponent* comp : allComps) {

		if (comp) {
			USkeletalMeshComponent* meshComp = Cast<USkeletalMeshComponent>(comp);
			if (meshComp) {
				UDebugMessages::LogError(this, "disabling for " + meshComp->GetName());
				meshComp->SetEnableGravity(false);
				meshComp->SetSimulatePhysics(false);
			}
		}
	}*/

	SetComponentTickEnabled(true);
}

void UClimbLadderComponent::FinishUp() {
	SetComponentTickEnabled(false);
}

void UClimbLadderComponent::SetMoveUp(bool val) {
	MoveUp = val;
}
void UClimbLadderComponent::SetPostClimb(bool val) {
	PostClimbing = val;
}


void UClimbLadderComponent::SetPreFallMove(bool val) {
	PreFallMove = val;
}
void UClimbLadderComponent::SetFallMove(bool val) {
	FallMove = val;
}

void UClimbLadderComponent::ClimbUpTick(float DeltaTime) {
	if (MoveUp) {
		auto newUpLocation = GetOwner()->GetActorLocation() + FVector(0, 0, 275);

		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), newUpLocation, DeltaTime, 75);
		GetOwner()->SetActorLocation(newLocation);
	}

	if (PreClimbing) {
		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), PreClimbLocation, DeltaTime, 50);
		GetOwner()->SetActorLocation(newLocation);

		if (FVector::Dist(PreClimbLocation, GetOwner()->GetActorLocation()) < 1)
			PreClimbing = false;
	}

	if (PostClimbing) {
		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), PostClimbLocation, DeltaTime, 125);
		newLocation.Z = PostClimbLocation.Z;
		GetOwner()->SetActorLocation(newLocation);

		UDebugMessages::LogDisplay(this, "post climb " + newLocation.ToString() + "||" + PostClimbLocation.ToString());

		AnimInstance->SetIsClimbing(false);
		if (FVector::Dist(PostClimbLocation, GetOwner()->GetActorLocation()) < 2) {
			PostClimbing = false;
			FinishUp();

			auto gridMovement = GetOwner()->GetComponentByClass<UGridMovementComponent>();
			gridMovement->MoveAcrossGridPostClimb();

			UDebugMessages::LogDisplay(this, "post climb complete " + PostClimbLocation.ToString());

			auto capsule = GetOwner()->GetComponentByClass<UCapsuleComponent>();
			capsule->SetEnableGravity(true);

			auto characterMovement = GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
			characterMovement->GravityScale = 1;

			GetOwner()->SetActorEnableCollision(true);
		}
	}
}
void UClimbLadderComponent::ClimbDownTick(float DeltaTime) {
	if (PreFallMove) {
		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 20), DeltaTime, 75);
		GetOwner()->SetActorLocation(newLocation);
	}

	if (FallMove) {
		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() - FVector(0, 0, 100), DeltaTime, 320);
		GetOwner()->SetActorLocation(newLocation);
	}

	if (PostClimbing) {
		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), PostClimbLocation, DeltaTime, 125);
		newLocation.Z = PostClimbLocation.Z;
		GetOwner()->SetActorLocation(newLocation);

		UDebugMessages::LogDisplay(this, "post climb " + newLocation.ToString() + "||" + PostClimbLocation.ToString());

		AnimInstance->SetIsClimbing(false);
		if (FVector::Dist(PostClimbLocation, GetOwner()->GetActorLocation()) < 2) {
			PostClimbing = false;
			FinishUp();

			auto gridMovement = GetOwner()->GetComponentByClass<UGridMovementComponent>();
			gridMovement->MoveAcrossGridPostClimb();

			UDebugMessages::LogDisplay(this, "post climb complete " + PostClimbLocation.ToString());

			auto capsule = GetOwner()->GetComponentByClass<UCapsuleComponent>();
			capsule->SetEnableGravity(true);

			auto characterMovement = GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
			characterMovement->GravityScale = 1;

			GetOwner()->SetActorEnableCollision(true);
		}
	}
}