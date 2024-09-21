// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimAccessComponent.h"
#include "../../BaseCharacter.h"
#include "../../Anim/CharAnimInstance.h"
#include "../../Anim/WorldCharAnimInstance.h"
#include "../../../Player/WorldPawn.h"

// Sets default values for this component's properties
UAnimAccessComponent::UAnimAccessComponent(const FObjectInitializer& ObjectInitializer)
	:UActorComponent(ObjectInitializer)
{

}


// Called when the game starts
void UAnimAccessComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ABaseCharacter* character = Cast<ABaseCharacter>(GetOwner());
	if (character)
		AnimInstance = Cast<UCharAnimInstance>(character->GetMesh()->GetAnimInstance());

	AWorldPawn* worldPawn = Cast<AWorldPawn>(GetOwner());
	if (worldPawn)
		WorldAnimInstance = Cast<UWorldCharAnimInstance>(worldPawn->GetSkeleMeshComponent()->GetAnimInstance());

}


// Called every frame
void UAnimAccessComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAnimAccessComponent::SetAnimInstanceRate(float rate) {
	AnimInstance->UpdateAnimPlayRate(rate);
}