// Fill out your copyright notice in the Description page of Project Settings.


#include "GunFireComponent.h"
#include "Components/StaticMeshComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Bullets/Bullet.h"
#include "../../Bullets/Components/TravelComponent.h"
#include "../../Bullets/Components/BulletDetailsComponent.h"
#include "../../../Utilities/GridUtilities.h"
#include "../../Components/EquipmentDetailsComponent.h"

// Sets default values for this component's properties
UGunFireComponent::UGunFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGunFireComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UGunFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

void UGunFireComponent::FireAtLocation(FVector location) {
	auto bulletStart = GetGunFireStartLocation();

	auto bulletRotation = UGridUtilities::FindLookAtRotation(bulletStart, location);

	auto newBullet = GetOwner()->GetWorld()->SpawnActor<ABullet>(bulletStart, bulletRotation);
	if (newBullet) {

		auto equipmentDetails = GetOwner()->GetComponentByClass<UEquipmentDetailsComponent>();
		auto bulletDetails = newBullet->GetComponentByClass<UBulletDetailsComponent>();
		if (bulletDetails && equipmentDetails)
			bulletDetails->SetBaseDamage(equipmentDetails->GetBaseDamage());

		auto bulletTravel = newBullet->GetComponentByClass<UTravelComponent>();
		if (bulletTravel)
			bulletTravel->StartTravel(location);
	}
}