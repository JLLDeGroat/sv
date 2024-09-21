// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AttackComponent.h"
#include "../../Equipment/Guns/PeaRifle.h"
#include "../../Equipment/Guns/UrfGun.h"
#include "../../Equipment/Bullets/Bullet.h"
#include "../../Equipment/Guns/Components/GunFireComponent.h"
#include "../../Equipment/Guns/Components/LightAttachmentComponent.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Equipment/Bullets/Components/TravelComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Equipment/Equipment.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Equipment/Components/AttachedVectorComponent.h"
#include "../Anim/CharAnimInstance.h"
#include "../../Utilities/SvUtilities.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEquipmentComponent::EquipPrimary(EGun gunType) {
	if (gunType == EGun::INVALID)
		return UDebugMessages::LogError(this, "Attempted to equip a invalid guntype");

	UClass* cls = nullptr;
	switch (gunType) {
	case EGun::G_PeaRifle:
		cls = APeaRifle::StaticClass();
		break;
	}

	if (!cls) return UDebugMessages::LogError(this, "Invalid primary set, will not attach primary");

	auto gun = GetOwner()->GetWorld()->SpawnActor<AEquipment>(cls);
	auto skeleMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	if (skeleMesh) {
		gun->AttachToComponent(skeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));

		auto attachVectors = gun->GetComponentByClass<UAttachedVectorComponent>();
		if (attachVectors)
			attachVectors->UseAttachedVectors();

		Equipment.Emplace(gun);
		CurrentMainEquipment = gun;

		auto details = gun->GetComponentByClass<UEquipmentDetailsComponent>();
		details->SetIsPrimaryEquipment(true);
	}
}

void UEquipmentComponent::EquipSecondary(EGun gunType) {
	if (gunType == EGun::INVALID)
		return UDebugMessages::LogError(this, "Attmpted to equip a secondary invalid gun type");

	UClass* cls = nullptr;
	switch (gunType) {
	case EGun::G_UrfGun:
		cls = AUrfGun::StaticClass();
		break;
	}

	if (!cls) return UDebugMessages::LogError(this, "Invalid primary set, will not attach secondary");

	auto gun = GetOwner()->GetWorld()->SpawnActor<AUrfGun>(cls);
	auto skeleMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	if (skeleMesh) {
		gun->AttachToComponent(skeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("SecondaryHolsterSocket"));
		Equipment.Emplace(gun);

		auto attachedVectorComponent = gun->GetComponentByClass<UAttachedVectorComponent>();
		if (attachedVectorComponent)
			attachedVectorComponent->UseHolstedVectors();

		auto details = gun->GetComponentByClass<UEquipmentDetailsComponent>();
		details->SetIsSecondaryEquipment(true);

		auto lightComponent = gun->GetComponentByClass<ULightAttachmentComponent>();
		if (lightComponent) {
			lightComponent->SwitchOff();
		}
	}
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::UpdateActorVisibility(bool value) {
	for (int i = 0; i < Equipment.Num(); i++) {
		Equipment[i]->SetActorHiddenInGame(!value);
	}
}

void UEquipmentComponent::FireEquippedGun() {
	auto attackComponent = GetOwner()->GetComponentByClass<UAttackComponent>();
	if (attackComponent) {
		//TODO:
		//assuming equipped gun is an AR
		//assuming AR uses basic bullets
		/*auto equipment = Equipment[0];*/
		auto gunFire = CurrentMainEquipment->GetComponentByClass<UGunFireComponent>();
		if (gunFire)
			gunFire->FireAtLocation(attackComponent->GetCurrentTargetLocation(), attackComponent->GetCurrentAttackRandomRadius());
	}
}

int UEquipmentComponent::GetActionPointsNeededToUseEquipment() {
	//TODO: 
	// set it so that we get the amount from the equipment component
	return 2;
}

TArray<AEquipment*> UEquipmentComponent::GetAllMeleeEquipment() {
	TArray<AEquipment*> foundEquipment;
	for (int i = 0; i < Equipment.Num(); i++) {
		if (!Equipment[i]) continue;

		auto equipmentDetailsComponent = Equipment[i]->GetComponentByClass<UEquipmentDetailsComponent>();
		if (!equipmentDetailsComponent) {
			UDebugMessages::LogError(this, "This equipment has no details component");
			continue;
		}

		if (equipmentDetailsComponent->GetIsMelee())
			foundEquipment.Emplace(Equipment[i]);
	}
	return foundEquipment;
}

AEquipment* UEquipmentComponent::GetPrimaryEquipment() {
	for (int i = 0; i < Equipment.Num(); i++) {
		if (Equipment[i]) {
			auto detailsComponent = Equipment[i]->GetComponentByClass<UEquipmentDetailsComponent>();
			if (detailsComponent && detailsComponent->GetIsPrimaryEquipment())
				return Equipment[i];
		}
	}

	return nullptr;
}

AEquipment* UEquipmentComponent::GetSecondaryEquipment() {
	for (int i = 0; i < Equipment.Num(); i++) {
		if (Equipment[i]) {
			auto detailsComponent = Equipment[i]->GetComponentByClass<UEquipmentDetailsComponent>();
			if (detailsComponent && detailsComponent->GetIsSecondaryEquipment())
				return Equipment[i];
		}
	}

	return nullptr;
}

bool UEquipmentComponent::GetCanSwapWeapon() {
	return GetSecondaryEquipment() && GetPrimaryEquipment();
}

void UEquipmentComponent::SwapWeapon() {
	auto primary = GetPrimaryEquipment();
	auto secondary = GetSecondaryEquipment();

	auto secondaryDetails = secondary->GetComponentByClass<UEquipmentDetailsComponent>();
	auto primaryDetails = primary->GetComponentByClass<UEquipmentDetailsComponent>();

	EGun gunMovingTo = EGun::INVALID;
	if (CurrentMainEquipment == primary) {
		gunMovingTo = secondaryDetails->GetGunType();
	}
	else if (CurrentMainEquipment == secondary) {
		gunMovingTo = primaryDetails->GetGunType();
	}
	else
		return UDebugMessages::LogError(this, "failed to swap weapon, current main equipment is not primary of secondary");

	if (gunMovingTo == EGun::INVALID)
		return UDebugMessages::LogError(this, "failed to get gun moving to, gun moving to was invalid");

	auto animState = GetAnimStateFromGunType(gunMovingTo);
	if (animState == ECharacterAnimState::INVALID)
		return UDebugMessages::LogError(this, "failed to get character anim state");

	UDebugMessages::LogDisplay(this, "starting anim swap");
	AnimInstance->SetCharacterAnimState(animState);
}

void UEquipmentComponent::FinishSwapWeapon() {
	if (CurrentMainEquipment == GetPrimaryEquipment())
		CurrentMainEquipment = GetSecondaryEquipment();
	else if (CurrentMainEquipment == GetSecondaryEquipment())
		CurrentMainEquipment = GetPrimaryEquipment();
}

void UEquipmentComponent::HolsterCurrentMainEquipment() {
	auto mainDetails = CurrentMainEquipment->GetComponentByClass<UEquipmentDetailsComponent>();
	auto attachType = mainDetails->GetHolsterAttachType();

	auto lightAttachment = CurrentMainEquipment->GetComponentByClass<ULightAttachmentComponent>();
	if (lightAttachment)
		lightAttachment->SwitchOff();

	AttachEquipmentToSocket(attachType, CurrentMainEquipment);
}
void UEquipmentComponent::UnholsterNewMainEquipment() {
	AEquipment* equipmentToUnholster = nullptr;
	if (CurrentMainEquipment == GetPrimaryEquipment())
		equipmentToUnholster = GetSecondaryEquipment();
	else if (CurrentMainEquipment == GetSecondaryEquipment())
		equipmentToUnholster = GetPrimaryEquipment();
	else
		return UDebugMessages::LogError(this, "failed to get current main equipment, cannot unholster");

	auto lightAttachment = equipmentToUnholster->GetComponentByClass<ULightAttachmentComponent>();
	if (lightAttachment)
		lightAttachment->SwitchOn();

	AttachEquipmentToSocket(EAttachType::AT_RightHand, equipmentToUnholster);
}

void UEquipmentComponent::AttachEquipmentToSocket(EAttachType attachmentType, AEquipment* equipment) {
	auto skeleMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	equipment->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	auto attachSockeName = USvUtilities::GetSocketNameFromAttachment(attachmentType);

	equipment->AttachToComponent(skeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(attachSockeName));

	auto vectorComponent = equipment->GetComponentByClass<UAttachedVectorComponent>();

	FVector loc;
	FRotator rot;
	if (vectorComponent->GetAttachmentsForType(attachmentType, loc, rot)) {
		equipment->SetActorRelativeRotation(rot);
		equipment->SetActorRelativeLocation(loc);
	}
}

void UEquipmentComponent::ReloadEquipment() {
	AnimInstance->SetIsReloading(true);
}

ECharacterAnimState UEquipmentComponent::GetAnimStateFromGunType(EGun gunType) {
	switch (gunType) {
	case EGun::G_UrfGun:
		return ECharacterAnimState::AS_PISTOL;
	case EGun::G_PeaRifle:
		return ECharacterAnimState::AS_AR;
	}

	return ECharacterAnimState::INVALID;
}