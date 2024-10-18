// Fill out your copyright notice in the Description page of Project Settings.


#include "GunFireComponent.h"
#include "Components/StaticMeshComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Bullets/Base/BaseBullet.h"
#include "../../Bullets/Bullet.h"
#include "../../Bullets/Components/TravelComponent.h"
#include "../../Bullets/Components/BulletDetailsComponent.h"
#include "../../../Utilities/GridUtilities.h"
#include "../../Components/EquipmentDetailsComponent.h"
#include "MuzzleFlashComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGunFireComponent::UGunFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


void UGunFireComponent::SetBulletClass(TSubclassOf<ABaseBullet> clss) {
	BulletClass = clss;
}

FVector UGunFireComponent::GetGunFireStartLocation() {
	if (!GunFireStartMesh || GunFireStartSocketName == "") {
		UDebugMessages::LogError(this, "no gun fire start location set");
		return FVector::ZeroVector;
	}

	return GunFireStartMesh->GetSocketLocation(FName(GunFireStartSocketName));
}

void UGunFireComponent::SetMeshAndSocketName(UStaticMeshComponent* meshComponent, FString socketName) {
	GunFireStartMesh = meshComponent;
	GunFireStartSocketName = socketName;
}

void UGunFireComponent::FireAtLocation(FVector location, float accuracyRadius) {
	auto owner = GetOwner();

	auto bulletStart = GetGunFireStartLocation();

	auto equipmentDetails = owner->GetComponentByClass<UEquipmentDetailsComponent>();
	auto randomUnitVector = FVector(FMath::RandRange(0, 1), FMath::RandRange(0, 1), FMath::RandRange(0, 1));
	auto randomFloatInRange = FMath::RandRange(0.00f, accuracyRadius);
	auto offsetLoc = randomUnitVector * randomFloatInRange;
	auto finalLoc = location + offsetLoc;

	//DrawDebugLine(GetOwner()->GetWorld(), bulletStart, finalLoc, FColor::Green, true, 60, 0, 1);

	auto bulletRotation = UGridUtilities::FindLookAtRotation(bulletStart, finalLoc);

	UDebugMessages::LogDisplay(this, "spawning bullet at: " + bulletStart.ToString());

	if (!BulletClass)
		return UDebugMessages::LogError(this, "failed to get bullet class");

	auto newBullet = owner->GetWorld()->SpawnActor<ABaseBullet>(BulletClass, bulletStart, bulletRotation);
	auto muzzleFlash = owner->GetComponentByClass<UMuzzleFlashComponent>();

	if (newBullet) {
		auto bulletDetails = newBullet->GetComponentByClass<UBulletDetailsComponent>();
		if (bulletDetails && equipmentDetails) {
			bulletDetails->SetBaseDamage(Random.RandRange(equipmentDetails->GetMinBaseDamage(), equipmentDetails->GetMaxBaseDamage()));
			bulletDetails->SetGunShotFrom(owner);
			bulletDetails->CompleteSetup();
		}
		auto bulletTravel = newBullet->GetComponentByClass<UTravelComponent>();
		if (bulletTravel)
			bulletTravel->StartTravel(finalLoc);

		if (muzzleFlash)
			muzzleFlash->ActivateMuzzleFlash();
	}
}