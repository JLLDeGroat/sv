

#include "EquipmentComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AttackComponent.h"
#include "../../Equipment/Guns/PeaRifle.h"
#include "../../Equipment/Guns/UrfGun.h"
#include "../../Equipment/Guns/ConstructRifle.h"
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
#include "../../Equipment/Guns/Components/GunActivationComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Equipment/Clothing/ClothingHelpers.h"
#include "../../Equipment/Clothing/Clothing.h"
#include "../../Equipment/Clothing/Components/ClothingAttachComponent.h"
#include "StatusEffectsComponent.h"

DEFINE_LOG_CATEGORY(SVLogEquipmentComponent);

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent(const FObjectInitializer &ObjectInitializer)
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

void UEquipmentComponent::EquipPrimary(EGun gunType)
{
	if (gunType == EGun::INVALID)
		return UDebugMessages::LogError(this, "Attempted to equip a invalid guntype");

	UClass *cls = nullptr;
	switch (gunType)
	{
	case EGun::G_PeaRifle:
		cls = APeaRifle::StaticClass();
		break;
	case EGun::G_ConstructRifle:
		cls = AConstructRifle::StaticClass();
		break;
	}

	if (!cls)
		return UDebugMessages::LogError(this, "Invalid primary set, will not attach primary");

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = GetOwner();

	auto gun = GetOwner()->GetWorld()->SpawnActor<AEquipment>(cls, spawnParams);
	auto skeleMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();

	auto gunDetailsComponent = gun->GetComponentByClass<UEquipmentDetailsComponent>();

	if (skeleMesh)
	{
		/*gun->AttachToComponent(skeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));*/
		gun->AttachToComponent(skeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
							   FName(USvUtilities::GetSocketNameFromAttachment(gunDetailsComponent->GetEquipSocket())));

		if (gunDetailsComponent->GetRelativeScaleOnEquip() != FVector::ZeroVector)
			gun->SetActorRelativeScale3D(gunDetailsComponent->GetRelativeScaleOnEquip());

		auto attachVectors = gun->GetComponentByClass<UAttachedVectorComponent>();
		if (attachVectors)
			attachVectors->UseAttachedVectors();

		Equipment.Emplace(gun);
		CurrentMainEquipment = gun;

		auto details = gun->GetComponentByClass<UEquipmentDetailsComponent>();
		details->SetIsPrimaryEquipment(true);
	}
}

void UEquipmentComponent::EquipSecondary(EGun gunType)
{
	if (gunType == EGun::INVALID)
		return UDebugMessages::LogError(this, "Attmpted to equip a secondary invalid gun type");

	UClass *cls = nullptr;
	switch (gunType)
	{
	case EGun::G_UrfGun:
		cls = AUrfGun::StaticClass();
		break;
	}

	if (!cls)
		return UDebugMessages::LogError(this, "Invalid primary set, will not attach secondary");

	auto gun = GetOwner()->GetWorld()->SpawnActor<AUrfGun>(cls);
	auto skeleMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	if (skeleMesh)
	{
		gun->AttachToComponent(skeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("SecondaryHolsterSocket"));
		Equipment.Emplace(gun);

		auto attachedVectorComponent = gun->GetComponentByClass<UAttachedVectorComponent>();
		if (attachedVectorComponent)
			attachedVectorComponent->UseHolstedVectors();

		auto details = gun->GetComponentByClass<UEquipmentDetailsComponent>();
		details->SetIsSecondaryEquipment(true);

		auto lightComponent = gun->GetComponentByClass<ULightAttachmentComponent>();
		if (lightComponent)
		{
			lightComponent->SwitchOff();
		}
	}
}

// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::UpdateActorVisibility(bool value)
{
	for (int i = 0; i < Equipment.Num(); i++)
	{
		Equipment[i]->SetActorHiddenInGame(!value);
	}
}

void UEquipmentComponent::FireEquippedGun()
{
	auto attackComponent = GetOwner()->GetComponentByClass<UAttackComponent>();
	if (attackComponent)
	{
		if (!CurrentMainEquipment)
			return UDebugMessages::LogError(this, "cannot fire gun, there is no current main equipment");

		// TODO:
		// assuming equipped gun is an AR
		// assuming AR uses basic bullets
		/*auto equipment = Equipment[0];*/
		auto gunFire = CurrentMainEquipment->GetComponentByClass<UGunFireComponent>();
		if (!gunFire)
			return UDebugMessages::LogDisplay(this, "current main equipment has no gunfire component");

		float debuff = 0.00f;
		auto statusEffectComponent = GetOwner()->GetComponentByClass<UStatusEffectsComponent>();
		if (!statusEffectComponent)
			UDebugMessages::LogWarning(this, "firer has no status effect component");
		else
		{
			statusEffectComponent->TryGetDebuffValue(EDebuffType::DBT_Accuracy, debuff);
			UDebugMessages::LogDisplay(this, "found debuff value of " + FString::SanitizeFloat(debuff, 2));
		}
		gunFire->FireAtLocation(attackComponent->GetCurrentTargetLocation(), (attackComponent->GetCurrentAttackRandomRadius() + debuff));
	}
}

int UEquipmentComponent::GetActionPointsNeededToUseEquipment()
{
	// TODO:
	//  set it so that we get the amount from the equipment component
	return 2;
}

TArray<AEquipment *> UEquipmentComponent::GetAllMeleeEquipment()
{
	TArray<AEquipment *> foundEquipment;
	for (int i = 0; i < Equipment.Num(); i++)
	{
		if (!Equipment[i])
			continue;

		auto equipmentDetailsComponent = Equipment[i]->GetComponentByClass<UEquipmentDetailsComponent>();
		if (!equipmentDetailsComponent)
		{
			UDebugMessages::LogError(this, "This equipment has no details component");
			continue;
		}

		if (equipmentDetailsComponent->GetIsMelee())
			foundEquipment.Emplace(Equipment[i]);
	}
	return foundEquipment;
}

TArray<AEquipment *> UEquipmentComponent::GetAllEquipment()
{
	return Equipment;
}
TArray<AClothing *> UEquipmentComponent::GetAllClothing()
{
	return Clothing;
}

AEquipment *UEquipmentComponent::GetPrimaryEquipment()
{
	for (int i = 0; i < Equipment.Num(); i++)
	{
		if (Equipment[i])
		{
			auto detailsComponent = Equipment[i]->GetComponentByClass<UEquipmentDetailsComponent>();
			if (detailsComponent && detailsComponent->GetIsPrimaryEquipment())
				return Equipment[i];
		}
	}

	return nullptr;
}

AEquipment *UEquipmentComponent::GetSecondaryEquipment()
{
	for (int i = 0; i < Equipment.Num(); i++)
	{
		if (Equipment[i])
		{
			auto detailsComponent = Equipment[i]->GetComponentByClass<UEquipmentDetailsComponent>();
			if (detailsComponent && detailsComponent->GetIsSecondaryEquipment())
				return Equipment[i];
		}
	}

	return nullptr;
}

bool UEquipmentComponent::GetCanSwapWeapon()
{
	return GetSecondaryEquipment() && GetPrimaryEquipment();
}

void UEquipmentComponent::SwapWeapon()
{
	auto primary = GetPrimaryEquipment();
	auto secondary = GetSecondaryEquipment();

	auto secondaryDetails = secondary->GetComponentByClass<UEquipmentDetailsComponent>();
	auto primaryDetails = primary->GetComponentByClass<UEquipmentDetailsComponent>();

	EGun gunMovingTo = EGun::INVALID;
	if (CurrentMainEquipment == primary)
	{
		gunMovingTo = secondaryDetails->GetGunType();
	}
	else if (CurrentMainEquipment == secondary)
	{
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

void UEquipmentComponent::FinishSwapWeapon()
{
	if (CurrentMainEquipment == GetPrimaryEquipment())
		CurrentMainEquipment = GetSecondaryEquipment();
	else if (CurrentMainEquipment == GetSecondaryEquipment())
		CurrentMainEquipment = GetPrimaryEquipment();
}

void UEquipmentComponent::HolsterCurrentMainEquipment()
{
	auto mainDetails = CurrentMainEquipment->GetComponentByClass<UEquipmentDetailsComponent>();
	auto attachType = mainDetails->GetHolsterAttachType();

	auto lightAttachment = CurrentMainEquipment->GetComponentByClass<ULightAttachmentComponent>();
	if (lightAttachment)
		lightAttachment->SwitchOff();

	AttachEquipmentToSocket(attachType, CurrentMainEquipment);
}
void UEquipmentComponent::UnholsterNewMainEquipment()
{
	AEquipment *equipmentToUnholster = nullptr;
	if (CurrentMainEquipment == GetPrimaryEquipment())
		equipmentToUnholster = GetSecondaryEquipment();
	else if (CurrentMainEquipment == GetSecondaryEquipment())
		equipmentToUnholster = GetPrimaryEquipment();
	else
		return UDebugMessages::LogError(this, "failed to get current main equipment, cannot unholster");

	auto lightAttachment = equipmentToUnholster->GetComponentByClass<ULightAttachmentComponent>();
	if (lightAttachment)
		lightAttachment->SwitchOn();

	auto equipmentDetails = equipmentToUnholster->GetComponentByClass<UEquipmentDetailsComponent>();

	AttachEquipmentToSocket(equipmentDetails->GetEquipSocket(), equipmentToUnholster);
}

void UEquipmentComponent::AttachEquipmentToSocket(EAttachType attachmentType, AEquipment *equipment)
{
	auto skeleMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	equipment->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	auto attachSockeName = USvUtilities::GetSocketNameFromAttachment(attachmentType);

	equipment->AttachToComponent(skeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(attachSockeName));

	auto vectorComponent = equipment->GetComponentByClass<UAttachedVectorComponent>();

	FVector loc;
	FRotator rot;
	if (vectorComponent->GetAttachmentsForType(attachmentType, loc, rot))
	{
		equipment->SetActorRelativeRotation(rot);
		equipment->SetActorRelativeLocation(loc);
	}
}

void UEquipmentComponent::ReloadEquipment()
{
	AnimInstance->SetIsReloading(true);
}

ECharacterAnimState UEquipmentComponent::GetAnimStateFromGunType(EGun gunType)
{
	switch (gunType)
	{
	case EGun::G_UrfGun:
		return ECharacterAnimState::AS_PISTOL;
	case EGun::G_PeaRifle:
		return ECharacterAnimState::AS_AR;
	}

	return ECharacterAnimState::INVALID;
}
#pragma optimize("", off)
void UEquipmentComponent::EquipClothing(EClothingType clothingType, EClothingGrade clothingGrade)
{
	UDebugMessages::LogDisplay(this, "Equipping Clothing", SVLogEquipmentComponent);
	auto clothingClass = UClothingHelpers::DetermineClothingClass(clothingType, clothingGrade);
	if (!clothingClass)
		return UDebugMessages::LogError(this, "failed to get clothing class", SVLogEquipmentComponent);

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = GetOwner();

	auto spawnedClothing = GetOwner()->GetWorld()->SpawnActor<AClothing>(clothingClass, spawnParams);
	if (!spawnedClothing)
		return UDebugMessages::LogError(this, "failed to spawn clothing", SVLogEquipmentComponent);

	auto skeleMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	if (!skeleMesh)
		return UDebugMessages::LogError(this, "failed to get skeletal mesh", SVLogEquipmentComponent);

	// no send the skeleton to the clothing so it can attach and set the correct vector locations

	auto clothingAttachComponent = spawnedClothing->GetComponentByClass<UClothingAttachComponent>();
	if (!clothingAttachComponent)
		return UDebugMessages::LogError(this, "failed to get clothing attach component", SVLogEquipmentComponent);

	auto attachments = clothingAttachComponent->GetAttachments();
	if (attachments.Num() == 0)
		return UDebugMessages::LogError(this, "failed to get attachments from clothing attach component", SVLogEquipmentComponent);

	for (auto attachment : attachments)
	{
		auto attachSocketName = USvUtilities::GetSocketNameFromAttachment(attachment.GetType());
		UDebugMessages::LogDisplay(this, "Attaching to  " + attachSocketName, SVLogEquipmentComponent);
		attachment.GetMesh()->AttachToComponent(skeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(attachSocketName));
		attachment.GetMesh()->SetRelativeLocation(attachment.GetOffset());
		attachment.GetMesh()->SetRelativeRotation(attachment.GetRotationOffset());
		attachment.GetMesh()->SetRelativeScale3D(attachment.GetScale());
	}

	UDebugMessages::LogDisplay(this, "Finished Equipping Clothing", SVLogEquipmentComponent);
	Clothing.Emplace(spawnedClothing);
}
#pragma optimize("", on)
void UEquipmentComponent::UnEquip(AEquipment *equipment)
{
	for (int i = 0; i < Equipment.Num(); i++)
	{
		if (Equipment[i] == equipment)
		{
			if (Equipment[i] == CurrentMainEquipment)
				CurrentMainEquipment = nullptr;

			Equipment.RemoveAt(i);
			return;
		}
	}
}

void UEquipmentComponent::TryActivateMainEquipment()
{
	if (CurrentMainEquipment)
	{
		auto activationComp = CurrentMainEquipment->GetComponentByClass<UGunActivationComponent>();
		if (activationComp)
		{
			activationComp->ActivateGun();
		}
	}
}
void UEquipmentComponent::TryDeactivateMainEquipment()
{
	if (CurrentMainEquipment)
	{
		auto activationComp = CurrentMainEquipment->GetComponentByClass<UGunActivationComponent>();
		if (activationComp)
		{
			activationComp->DeactivateGun();
		}
	}
}