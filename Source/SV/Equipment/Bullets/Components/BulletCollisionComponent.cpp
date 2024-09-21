// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletCollisionComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Characters/Components/HitCapsuleComponent.h"
#include "../../../Characters/Components/HitBoxComponent.h"
#include "../../../Characters/Components/DamageRecieveComponent.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"
#include "BulletHitSoundComponent.h"
#include "BulletDetailsComponent.h"
#include "TravelComponent.h"
#include "BulletRearComponent.h"
#include "../../../Effects/BulletHoleDecal.h"
#include "DrawDebugHelpers.h"

#include "../../../Environment/EnvironmentActor.h"
#include "../../../Environment/Components/EnvironmentDetailsComponent.h"


UBulletCollisionComponent::UBulletCollisionComponent(const FObjectInitializer& ObjectInitializer)
	: UCapsuleComponent(ObjectInitializer) {
	SetCanEverAffectNavigation(false);
	SetGenerateOverlapEvents(true);

	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionObjectType(USvUtilities::GetBulletCollisionObjectChannel());
	SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(USvUtilities::GetFloorTargetChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	OnComponentBeginOverlap.AddDynamic(this, &UBulletCollisionComponent::Overlapped);
}

void UBulletCollisionComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (bIsDisabled)
		return;

	UDebugMessages::LogDisplay(this, "overlapped " + OtherActor->GetName() + " comp: " + OtherComp->GetName());

	if (OtherComp->Implements<UHitComponent>()) {
		TScriptInterface<IHitComponent> hitComp = OtherComp;
		auto bulletDetails = GetOwner()->GetComponentByClass<UBulletDetailsComponent>();
		auto damageRecieve = OtherActor->GetComponentByClass<UDamageRecieveComponent>();
		auto otherCharacterDetails = OtherActor->GetComponentByClass<UCharacterDetailsComponent>();
		auto damageDone = 0;

		if (!bulletDetails || !damageRecieve || !otherCharacterDetails || bulletDetails->GetPenetrationAbility() <= 0) {
			bIsDisabled = true;
			GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &UBulletCollisionComponent::OnDestroyCallback, 5.0f, false);
			UDebugMessages::LogError(this, "no bullet details, damage recieve or character details, or has 0 penetration ERROR, will do no damage");
		}
		else {
			bulletDetails->RemoveFromPenetrationAbility(hitComp->GetThickness());
			if (!otherCharacterDetails->GetIsDead()) {
				damageDone = damageRecieve->DoDamage(hitComp->GetHitDamageMultiplier(), bulletDetails->GetBaseDamage(),
					bulletDetails->GetBaseImpulse(), GetOwner()->GetActorRotation());
				UDebugMessages::LogDisplay(this, "bullet did " + FString::SanitizeFloat(damageDone, 0) + " damage");
				AttemptToInitiateBulletSound();
			}
		}
		//TODO:
		//Create Blood spatter
		auto meshComponent = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
		if (meshComponent)
			meshComponent->SetVisibility(false);

		auto sourceGun = bulletDetails->GetGunShotFrom();
		if (!sourceGun || !sourceGun->GetAttachParentActor())
			UDebugMessages::LogError(this, "could not determine where bullet came from");
		else if (damageDone == 0)
			UDebugMessages::LogError(this, "Did 0 damage, not running stat task");
		else {
			auto firer = sourceGun->GetAttachParentActor();

			USvUtilities::AttemptToStartStatUpdater(firer, EStatisticType::ST_DAMAGE, damageDone);
			if (damageDone > 0 && otherCharacterDetails && otherCharacterDetails->GetIsDead())
				USvUtilities::AttemptToStartStatUpdater(firer, EStatisticType::ST_Kill);
		}

		if (bulletDetails->GetPenetrationAbility() <= 0) {
			bIsDisabled = true;
			UDebugMessages::LogDisplay(this, "bullet can no longer penetrate");
			DisableBullet();
		}
	}

	//TODO: change environment tests
	else if (auto environment = Cast<AEnvironmentActor>(OtherActor)) {
		//TODO: Create environment spatter
		auto meshComponent = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
		if (meshComponent)
			meshComponent->SetVisibility(false);

		auto reverseRotation = GetOwner()->GetActorRotation();
		reverseRotation.Yaw += 180;
		GetWorld()->SpawnActor<ABulletHoleDecal>(GetLocationToSpawnBulletHole(meshComponent), reverseRotation);

		auto bulletDetails = GetOwner()->GetComponentByClass<UBulletDetailsComponent>();
		auto environmentDetails = environment->GetComponentByClass<UEnvironmentDetailsComponent>();
		if (bulletDetails) {

			if (environmentDetails)
				bulletDetails->RemoveFromPenetrationAbility(environmentDetails->GetThickness());
			else
				bulletDetails->RemoveFromPenetrationAbility(INT32_MAX);

			if (bulletDetails->GetPenetrationAbility() <= 0) {
				UDebugMessages::LogDisplay(this, "bullet can no longer penetrate");
				DisableBullet();
			}
		}
	}
}

void UBulletCollisionComponent::AttemptToInitiateBulletSound() {
	auto hitComponent = GetOwner()->GetComponentByClass<UBulletHitSoundComponent>();
	if (hitComponent) {
		hitComponent->SetWorldLocation(GetOwner()->GetActorLocation());
		hitComponent->SetSphereRadius(800);
	}
}

void UBulletCollisionComponent::OnDestroyCallback() {
	UDebugMessages::LogDisplay(this, "Destroying bullet");
	GetOwner()->Destroy();
}

FVector UBulletCollisionComponent::GetLocationToSpawnBulletHole(UStaticMeshComponent* hitMesh) {
	FVector closestPoint;
	float Distance;

	auto rearComp = GetOwner()->GetComponentByClass<UBulletRearComponent>();
	auto queryLocation = rearComp ?
		rearComp->GetComponentLocation() :
		GetOwner()->GetActorLocation();

	if (!hitMesh) return GetOwner()->GetActorLocation();

	Distance = hitMesh->GetClosestPointOnCollision(queryLocation, closestPoint);

	if (Distance > 0) return closestPoint;

	FBoxSphereBounds MeshBounds = hitMesh->Bounds;
	if (MeshBounds.GetBox().IsValid)
	{
		closestPoint = MeshBounds.GetBox().GetClosestPointTo(queryLocation);
		return closestPoint;
	}

	FVector MeshOrigin = MeshBounds.Origin;
	float SphereRadius = MeshBounds.SphereRadius;
	FVector Direction = (queryLocation - MeshOrigin).GetSafeNormal();
	return MeshOrigin + Direction * SphereRadius;
}

void UBulletCollisionComponent::DisableBullet() {
	bIsDisabled = true;

	auto travelComp = GetOwner()->GetComponentByClass<UTravelComponent>();
	if (travelComp)
		travelComp->EndTravel();

	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &UBulletCollisionComponent::OnDestroyCallback, 5.0f, false);
}