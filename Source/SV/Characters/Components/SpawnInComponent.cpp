// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnInComponent.h"
#include "../Anim/CharAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

USpawnInComponent::USpawnInComponent(const FObjectInitializer& ObjectInitializer)
	:UAnimAccessComponent(ObjectInitializer) {

	PrimaryComponentTick.bCanEverTick = true;
	MovementUpSpeed = 25;
}

void USpawnInComponent::BeginPlay() {
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void USpawnInComponent::UpdateMovementSpeed(float speed) {
	MovementUpSpeed = speed;
}

void USpawnInComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), EndLocation, DeltaTime, MovementUpSpeed);
	GetOwner()->SetActorLocation(newLocation);

	if (FVector::Dist(GetOwner()->GetActorLocation(), EndLocation) < 1) {
		SetComponentTickEnabled(false);
		AnimInstance->SetIsSpawningFromGround(false);
	}
}

void USpawnInComponent::StartSpawnInFromGround() {
	AnimInstance->SetIsSpawningFromGround(true);

	auto characterOwner = GetOwner<ACharacter>();
	characterOwner->GetCharacterMovement()->DisableMovement();
	characterOwner->GetCapsuleComponent()->SetEnableGravity(false);


	EndLocation = characterOwner->GetActorLocation();
	characterOwner->SetActorLocation(EndLocation - FVector(0, 0, 74));
}