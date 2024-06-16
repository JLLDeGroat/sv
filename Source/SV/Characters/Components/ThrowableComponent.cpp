// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Instance/SvGameInstance.h"
#include "../Anim/CharAnimInstance.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Equipment/Throwable/Grenade.h"
#include "../../Equipment/Throwable/Components/ThrowTravelComponent.h"
#include "../../Equipment/Throwable/Components/ThrowExplosionComponent.h"
#include "../../Equipment/Components/AttachedVectorComponent.h"
#include "../../Equipment/Throwable/Components/ThrownOwnerComponent.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UThrowableComponent::UThrowableComponent(const FObjectInitializer& ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ThrownEquipment = nullptr;
}


// Called when the game starts
void UThrowableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UDebugMessages::LogWarning(this, "Debug method here adding one test grenade");
	AddThrowable(EThrowable::T_Grenade, 1);
	SetComponentTickEnabled(false);
}

// Called every frame
void UThrowableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto lookAtRot = UGridUtilities::FindLookAtRotation(GetOwner()->GetActorLocation(), ThrowingDestination);
	lookAtRot.Pitch = GetOwner()->GetActorRotation().Pitch;
	lookAtRot.Roll = GetOwner()->GetActorRotation().Roll;

	auto newRotation = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), lookAtRot, DeltaTime, 3);
	GetOwner()->SetActorRotation(newRotation);

	if ((newRotation.Yaw - 2.5f) < lookAtRot.Yaw &&
		(newRotation.Yaw + 2.5f) > lookAtRot.Yaw)
	{
		UDebugMessages::LogDisplay(this, "finished turning to throwing target");
		SetComponentTickEnabled(false);
	}

}

int UThrowableComponent::GetThrowableAmount(EThrowable throwable) {
	for (int i = 0; i < Throwables.Num(); i++) {
		if (Throwables[i].GetThrowable()->GetThrowable() == throwable)
			return Throwables[i].GetAmount();
	}
	return 0;
}

FThrowableDataItem* UThrowableComponent::GetThrowableItem(EThrowable throwable) {
	for (int i = 0; i < Throwables.Num(); i++) {
		if (Throwables[i].GetThrowable()->GetThrowable() == throwable)
			return Throwables[i].GetThrowable();
	}
	return nullptr;
}

void UThrowableComponent::AddThrowable(EThrowable throwable, int amount) {
	for (int i = 0; i < Throwables.Num(); i++) {
		if (Throwables[i].GetThrowable()->GetThrowable() == throwable)
			return Throwables[i].Add(amount);
	}

	auto instance = USvUtilities::GetGameInstance(GetOwner()->GetWorld());
	FThrowableDataItem dataItem;
	instance->GetThrowableDataItem(throwable, dataItem);
	Throwables.Emplace(FThrowable(dataItem, 1));
}

void UThrowableComponent::ThrowAtLocation(FVector location) {
	ThrowingDestination = location;
	AnimInstance->SetIsThrowing(true);
	SetComponentTickEnabled(true);
}

void UThrowableComponent::SpawnThrowableOfTypeAtRightHand(EThrowable throwable) {
	auto skeletalMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	if (!skeletalMesh) return UDebugMessages::LogError(this, "failed to get skele mesh for throwable spawning");

	FActorSpawnParameters param;
	param.Owner = GetOwner();
	ThrownEquipment = nullptr;
	if (throwable == EThrowable::T_Grenade)
		ThrownEquipment = GetOwner()->GetWorld()->SpawnActor<AGrenade>(param);
	else
		ThrownEquipment = GetOwner()->GetWorld()->SpawnActor<AGrenade>(param);

	if (!ThrownEquipment)
		return UDebugMessages::LogError(this, "failed to get spawned throwable");

	auto travelComp = ThrownEquipment->GetComponentByClass<UThrowTravelComponent>();
	travelComp->SetDestination(ThrowingDestination, skeletalMesh->GetSocketLocation(FName("RightHandSocket")));
	ThrownEquipment->AttachToComponent(skeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));

	auto attachedComponent = ThrownEquipment->GetComponentByClass<UAttachedVectorComponent>();
	if (attachedComponent) {
		auto attachedLocation = attachedComponent->GetAttachedLocation();
		auto attachedRotation = attachedComponent->GetAttachedRotation();

		ThrownEquipment->SetActorRelativeLocation(attachedLocation);
		ThrownEquipment->SetActorRelativeRotation(attachedRotation);
	}

	auto equipmentDetailsComponent = ThrownEquipment->GetComponentByClass<UEquipmentDetailsComponent>();
	auto characterDetailsComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (equipmentDetailsComponent && characterDetailsComponent)
		characterDetailsComponent->RemoveActionPoints(equipmentDetailsComponent->GetApCost());

	auto thrownOwner = ThrownEquipment->GetComponentByClass<UThrownOwnerComponent>();
	if (thrownOwner) thrownOwner->SetThrownOwner(GetOwner());

	auto explosionComp = ThrownEquipment->GetComponentByClass<UThrowExplosionComponent>();
	if (explosionComp) {
		auto throwableData = GetThrowableItem(throwable);
		explosionComp->SetExplosionRadius(throwableData->GetRadius());
	}

	AnimInstance->SetIsThrowing(false);
}

AEquipment* UThrowableComponent::GetThrownEquipment() {
	return ThrownEquipment;
}