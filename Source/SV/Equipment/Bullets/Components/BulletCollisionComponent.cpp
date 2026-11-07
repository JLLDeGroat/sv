// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletCollisionComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Characters/Components/HitCapsuleComponent.h"
#include "../../../Characters/Components/HitBoxComponent.h"
#include "../../../Characters/Components/DamageReceiveComponent.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../Characters/Components/AIComponent.h"
#include "../../../Characters/Components/EquipmentComponent.h"
#include "../../../Characters/BaseCharacter.h"
#include "BulletHitSoundComponent.h"
#include "BulletDetailsComponent.h"
#include "TravelComponent.h"
#include "BulletRearComponent.h"
#include "BulletAuraComponent.h"
#include "../../../Effects/BulletHoleDecal.h"
#include "DrawDebugHelpers.h"

#include "../../../Environment/EnvironmentActor.h"
#include "../../../Environment/Components/EnvironmentDetailsComponent.h"

#include "../../../Environment/Destructibles/Components/DestructibleDetails.h"
#include "../../../Environment/Destructibles/Components/DestructibleHitComponent.h"

#include "../../../Effects/BloodHitEffect.h"
#include "../../../Effects/BloodSpatterDecal.h"
#include "../../../Effects/ArmourHitEffect.h"

#include "../../../Equipment/Clothing/Components/ClothingDamageReceiveComponent.h"
#include "../../../Equipment/Equipment.h"
#include "../../../Equipment/Clothing/Clothing.h"

DEFINE_LOG_CATEGORY(SVLogBulletCollision)

UBulletCollisionComponent::UBulletCollisionComponent(const FObjectInitializer &ObjectInitializer)
	: UCapsuleComponent(ObjectInitializer)
{
	SetCanEverAffectNavigation(false);
	SetGenerateOverlapEvents(true);

	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionObjectType(USvUtilities::GetBulletCollisionObjectChannel());
	SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(USvUtilities::GetFloorTargetChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	ComponentId = FGuid::NewGuid();

	OnComponentBeginOverlap.AddDynamic(this, &UBulletCollisionComponent::Overlapped);
}
#pragma optimize("", off)
void UBulletCollisionComponent::Overlapped(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
										   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (bIsDisabled)
		return;

	auto bulletDetails = GetOwner()->GetComponentByClass<UBulletDetailsComponent>();
	if (!bulletDetails->GetIsSetup())
		return UDebugMessages::LogWarning(this, ComponentId.ToString() + ": " + "collision happened before setup, ignoring", SVLogBulletCollision);

	// DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), OtherComp->GetComponentLocation(), FColor::Red, false, 50, 0, 0);
	if (OtherComp->Implements<UHitComponent>())
	{
		if (FVector::Dist(GetOwner()->GetActorLocation(), OtherComp->GetComponentLocation()) > 50)
		{
			GetOwner()->SetActorLocation(OtherComp->GetComponentLocation());
			UDebugMessages::LogWarning(this, ComponentId.ToString() + ": " + "bullet was to far away, moved to the component", SVLogBulletCollision);
		}

		auto actorAndHitComponent = DetermineActorToDamageWithBullet(OtherComp, OtherActor);
		auto actorToDamage = actorAndHitComponent.Get<0>();
		auto hitComp = actorAndHitComponent.Get<1>();

		if (!actorToDamage || !hitComp)
		{
			UDebugMessages::LogError(this, ComponentId.ToString() + ": " + "failed collision", SVLogBulletCollision);
			return;
		}

		AddToAlreadyCollidedWith(actorToDamage);

		auto damageDone = 0;
		auto damageReceive = AttemptToGetActorsDamageReceieveComponent(actorToDamage);
		auto otherCharacterDetails = AttemptToGetActorsCharacterDetailsComponent(actorToDamage);

		if (!bulletDetails || !damageReceive || !otherCharacterDetails || bulletDetails->GetPenetrationAbility() <= 0)
		{
			bIsDisabled = true;
			GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &UBulletCollisionComponent::OnDestroyCallback, 5.0f, false);
			return UDebugMessages::LogError(this, ComponentId.ToString() + ": " + "no bullet details, damage Receive or character details, or has 0 penetration ERROR, will do no damage", SVLogBulletCollision);
		}
		else
		{
			bulletDetails->RemoveFromPenetrationAbility(hitComp->GetThickness());
			auto damageMultiplier = hitComp->GetHitDamageMultiplier();

			if (!otherCharacterDetails->GetIsDead())
			{
				auto damageMitigated = damageReceive->DamageToMitigateFromBullet(damageMultiplier, bulletDetails->GetBaseDamage());
				UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "bullet damage mitigated " + FString::SanitizeFloat(damageMitigated, 0) + " damage", SVLogBulletCollision);

				bulletDetails->AddDamageMitigation(damageMitigated);
				auto mitigatedBaseDamage = bulletDetails->GetBaseDamage() - bulletDetails->GetDamageMitigation();

				damageDone = damageReceive->DoDamage(damageMultiplier, mitigatedBaseDamage, bulletDetails->GetBaseImpulse(), GetOwner()->GetActorRotation());

				bulletDetails->AddToDamageDone(damageDone);

				UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "bullet did " + FString::SanitizeFloat(damageDone, 0) + " damage", SVLogBulletCollision);
				AttemptToInitiateBulletSound();

				if (hitComp->GetModularComponent())
					hitComp->DamageModularComponent(damageMultiplier * mitigatedBaseDamage, GetOwner()->GetActorRotation().Vector());
			}
		}
		// TODO:
		// Create Blood spatter

		if (damageDone > 0)
		{
			SpawnBloodEffects(hitComp, actorToDamage, otherCharacterDetails, bFromSweep, SweepResult.Location);

			auto sourceGun = bulletDetails->GetGunShotFrom();
			if (!sourceGun || !sourceGun->GetAttachParentActor())
				UDebugMessages::LogError(this, "could not determine where bullet came from", SVLogBulletCollision);
			else
			{
				auto firer = sourceGun->GetAttachParentActor();

				USvUtilities::AttemptToStartStatUpdater(firer, EStatisticType::ST_DAMAGE, damageDone);
				if (damageDone > 0 && otherCharacterDetails && otherCharacterDetails->GetIsDead())
					USvUtilities::AttemptToStartStatUpdater(firer, EStatisticType::ST_Kill);
			}

			// make bullet mesh invisible
			auto meshComponent = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
			if (meshComponent)
				meshComponent->SetVisibility(false);

			if (bulletDetails->GetPenetrationAbility() <= 0)
			{
				bIsDisabled = true;
				UDebugMessages::LogDisplay(this, "bullet can no longer penetrate", SVLogBulletCollision);
				DisableBullet();
			}
		}
	}

	// TODO: change environment tests
	else if (auto environment = Cast<AEnvironmentActor>(OtherActor))
	{
		// TODO: Create environment spatter
		auto meshComponent = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
		if (meshComponent)
			meshComponent->SetVisibility(false);

		auto reverseRotation = GetOwner()->GetActorRotation();
		reverseRotation.Yaw += 180;
		GetWorld()->SpawnActor<ABulletHoleDecal>(GetLocationToSpawnBulletHole(meshComponent), reverseRotation);

		auto environmentDetails = environment->GetComponentByClass<UEnvironmentDetailsComponent>();
		if (bulletDetails)
		{

			if (environmentDetails)
				bulletDetails->RemoveFromPenetrationAbility(environmentDetails->GetThickness());
			else
				bulletDetails->RemoveFromPenetrationAbility(INT32_MAX);

			if (bulletDetails->GetPenetrationAbility() <= 0)
			{
				UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "bullet can no longer penetrate", SVLogBulletCollision);
				DisableBullet();
			}
		}

		auto destructibleHit = environment->GetComponentByClass<UDestructibleHitComponent>();
		if (destructibleHit)
		{
			destructibleHit->ComponentWasHit(bulletDetails->GetBaseDamage(), GetOwner()->GetActorForwardVector(), GetOwner()->GetActorLocation(), 2500, 25, 10);
		}
	}
}

void UBulletCollisionComponent::AttemptToInitiateBulletSound()
{
	auto hitComponent = GetOwner()->GetComponentByClass<UBulletHitSoundComponent>();
	if (hitComponent)
	{
		hitComponent->SetWorldLocation(GetOwner()->GetActorLocation());
		hitComponent->SetSphereRadius(800);
	}
}

void UBulletCollisionComponent::OnDestroyCallback()
{
	UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "Destroying bullet", SVLogBulletCollision);
	GetOwner()->Destroy();
}

FVector UBulletCollisionComponent::GetLocationToSpawnBulletHole(UStaticMeshComponent *hitMesh)
{
	FVector closestPoint;
	float Distance;

	auto rearComp = GetOwner()->GetComponentByClass<UBulletRearComponent>();
	auto queryLocation = rearComp ? rearComp->GetComponentLocation() : GetOwner()->GetActorLocation();

	if (!hitMesh)
		return GetOwner()->GetActorLocation();

	Distance = hitMesh->GetClosestPointOnCollision(queryLocation, closestPoint);

	if (Distance > 0)
		return closestPoint;

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

FVector UBulletCollisionComponent::GetLocationToSpawnBloodHitEffect(UPrimitiveComponent *hitComponent, const FVector &entryLocation, const FVector &travelDirection)
{
	if (!hitComponent)
		return entryLocation;

	const FVector direction = travelDirection.GetSafeNormal();
	if (direction.IsNearlyZero())
		return entryLocation;

	const FBoxSphereBounds bounds = hitComponent->Bounds;
	const float distanceToBeyondBounds = FVector::DotProduct(bounds.Origin - entryLocation, direction) + bounds.SphereRadius + 2.0f;
	const FVector traceStart = entryLocation + direction * FMath::Max(distanceToBeyondBounds, 2.0f);
	FHitResult exitHit;
	FCollisionQueryParams queryParams;

	if (hitComponent->LineTraceComponent(exitHit, traceStart, entryLocation, queryParams))
		return exitHit.ImpactPoint + direction * 2.0f;

	FVector closestPoint;
	if (hitComponent->GetClosestPointOnCollision(traceStart, closestPoint) >= 0.0f)
		return closestPoint + direction * 2.0f;

	UDebugMessages::LogWarning(this, "could not trace the hit component for a blood effect exit point; using its forward bound", SVLogBulletCollision);
	const float forwardExtent = FVector::DotProduct(direction.GetAbs(), bounds.BoxExtent);
	return bounds.Origin + direction * (forwardExtent + 2.0f);
}

void UBulletCollisionComponent::DisableBullet()
{
	bIsDisabled = true;

	auto travelComp = GetOwner()->GetComponentByClass<UTravelComponent>();
	if (travelComp)
		travelComp->EndTravel();

	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &UBulletCollisionComponent::OnDestroyCallback, 5.0f, false);

	auto auraComp = GetOwner()->GetComponentByClass<UBulletAuraComponent>();
	if (auraComp)
	{
		auraComp->SetFloatParameter(FName("MinSize"), 0);
		auraComp->SetFloatParameter(FName("MaxSize"), 0);
		auraComp->SetFloatParameter(FName("SpecularScale"), 0);
		auraComp->SetFloatParameter(FName("RadiusSize"), 0);
	}
}

TScriptInterface<IDamageReceiver> UBulletCollisionComponent::AttemptToGetActorsDamageReceieveComponent(AActor *otherActor)
{
	for (auto component : otherActor->GetComponents())
	{
		if (component->IsA<UDamageReceiveComponent>() ||
			component->IsA<UClothingDamageReceiveComponent>())
			return component;
	}

	return nullptr;
}

UCharacterDetailsComponent *UBulletCollisionComponent::AttemptToGetActorsCharacterDetailsComponent(AActor *otherActor)
{
	// assuming actor is the character
	auto comp = otherActor->GetComponentByClass<UCharacterDetailsComponent>();
	if (!comp)
	{
		// now assuming its clothing
		auto clothingOwner = otherActor->GetOwner();
		comp = clothingOwner->GetComponentByClass<UCharacterDetailsComponent>();
	}

	return comp;
}

TArray<AActor *> UBulletCollisionComponent::GetActorsForBloodSpatterTraceToIgnore(AActor *otherActor)
{
	TArray<AActor *> actors;

	auto equipmentComponent = otherActor->GetComponentByClass<UEquipmentComponent>();
	if (!equipmentComponent)
	{
		// assumed its equipment
		auto owner = otherActor->GetOwner();
		if (owner)
		{
			equipmentComponent = owner->GetComponentByClass<UEquipmentComponent>();
			actors.Emplace(owner);
		}
	}

	if (equipmentComponent)
	{
		actors.Emplace(otherActor);
		for (auto equipment : equipmentComponent->GetAllEquipment())
			actors.Emplace(equipment);

		for (auto clothing : equipmentComponent->GetAllClothing())
			actors.Emplace(clothing);
	}

	return actors;
}

TTuple<AActor *, TScriptInterface<IHitComponent>> UBulletCollisionComponent::DetermineActorToDamageWithBullet(UPrimitiveComponent *component, AActor *otherActor)
{
	auto MakeResult = [](AActor *actor, UPrimitiveComponent *hitComponent)
	{
		TScriptInterface<IHitComponent> hitInterface;
		if (hitComponent && hitComponent->GetClass()->ImplementsInterface(UHitComponent::StaticClass()))
		{
			hitInterface.SetObject(hitComponent);
			hitInterface.SetInterface(Cast<IHitComponent>(hitComponent));
		}

		return MakeTuple(actor, hitInterface);
	};

	if (otherActor->IsA<ACharacter>())
	{
		// we found that its hit the soldier directly

		UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "starting multi scan", SVLogBulletCollision);

		TArray<FHitResult> Hits;
		GetCollisionSweepForBulletCollision(Hits);

		UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "going to get the first armor segment", SVLogBulletCollision);
		for (const FHitResult &Hit : Hits)
		{
			UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "hit actor " + Hit.GetActor()->GetName() + " | " + Hit.GetComponent()->GetName(), SVLogBulletCollision);
			if (Hit.GetActor()->IsA<AClothing>())
			{
				UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "hit armor segment " + Hit.GetActor()->GetName() + " | " + Hit.GetComponent()->GetName(), SVLogBulletCollision);
				if (ActorAlreadyCollidedWith(Hit.GetActor()))
				{
					UDebugMessages::LogWarning(this, ComponentId.ToString() + ": " + "Already collided with armor, use person", SVLogBulletCollision);
					return MakeResult(Hit.GetActor(), Hit.GetComponent());
				}
				else
				{
					// use this armor set piece
					UDebugMessages::LogWarning(this, ComponentId.ToString() + ": " + "Found Armour piece " + Hit.GetComponent()->GetName(), SVLogBulletCollision);
					return MakeResult(Hit.GetActor(), Hit.GetComponent());
				}
				break; // only deal with the first armour hit
			}
		}
		UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "ending multi scan counldnt find armour, going to harm character", SVLogBulletCollision);
		return MakeResult(otherActor, component);
	}
	else
	{
		UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "natively hit armor, this is cool", SVLogBulletCollision);
		if (ActorAlreadyCollidedWith(otherActor))
		{
			UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "already collided with natively hit armour", SVLogBulletCollision);
			TArray<FHitResult> nearbyHits;
			GetCollisionSweepForBulletCollision(nearbyHits);

			for (auto Hit : nearbyHits)
			{
				if (Hit.GetActor()->IsA<ACharacter>() && Hit.GetComponent()->Implements<UHitComponent>())
				{
					UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "found character near armor", SVLogBulletCollision);
					return MakeResult(Hit.GetActor(), Hit.GetComponent());
				}
			}
		}
		else
		{
			UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "first time hitting natively hit armour", SVLogBulletCollision);
			return MakeResult(otherActor, component);
		}
	}

	UDebugMessages::LogDisplay(this, ComponentId.ToString() + ": " + "could not determine a hit type, both are nullptr " + otherActor->GetName(), SVLogBulletCollision);

	return MakeResult(nullptr, nullptr);
}

void UBulletCollisionComponent::GetCollisionSweepForBulletCollision(TArray<FHitResult> &OutHits)
{
	const FVector Start = GetOwner()->GetActorLocation();
	const FVector End = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 25);
	TArray<FHitResult> Hits;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(BulletSweep), false, GetOwner());
	FCollisionObjectQueryParams ObjectTypes;

	GetWorld()->SweepMultiByObjectType(
		Hits,
		Start,
		End,
		GetOwner()->GetActorQuat(),
		ObjectTypes,
		FCollisionShape::MakeCapsule(
			GetScaledCapsuleRadius(),
			GetScaledCapsuleHalfHeight()),
		QueryParams);

	Hits.Sort([](const FHitResult &A, const FHitResult &B)
			  { return A.Time < B.Time; });

	OutHits = Hits;
}

bool UBulletCollisionComponent::ActorAlreadyCollidedWith(AActor *testActor)
{
	for (auto collidedWithActor : CollidedWith)
		if (collidedWithActor == testActor)
			return true;

	return false;
}

void UBulletCollisionComponent::AddToAlreadyCollidedWith(AActor *actor)
{
	CollidedWith.Emplace(actor);
}

EBloodEffect UBulletCollisionComponent::GetEffectsRequiredForCollision(AActor *otherActor)
{
	if (otherActor->IsA<ACharacter>())
	{
		auto aiComp = otherActor->GetComponentByClass<UAIComponent>();
		if (aiComp)
		{
			UDebugMessages::LogError(this, "this is not done", SVLogBulletCollision);
			return EBloodEffect::INVALID;
		}
		else
			return EBloodEffect::BE_Blood;
	}

	if (otherActor->IsA<AClothing>())
	{
		auto clothing = Cast<AClothing>(otherActor);
		auto grade = clothing->GetClothingGrade();

		switch (grade)
		{
		case EClothingGrade::CG_Iron:
			return EBloodEffect::BE_IronSpark;

		default:
			return EBloodEffect::BE_IronSpark;
		}
	}

	UDebugMessages::LogError(this, "this should not have happened", SVLogBulletCollision);
	return EBloodEffect::INVALID;
}

void UBulletCollisionComponent::SpawnBloodEffects(TScriptInterface<IHitComponent> hitComponent, AActor *bloodEffectActor, UCharacterDetailsComponent *detailsComponent,
												  bool bFromSweep, FVector sweepLoc)
{
	auto effectToSpawn = GetEffectsRequiredForCollision(bloodEffectActor);

	if (effectToSpawn == EBloodEffect::BE_IronSpark)
	{
		auto wallSpatterActor = GetWorld()->SpawnActor<AArmourHitEffect>(GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
		if (!wallSpatterActor)
			UDebugMessages::LogError(this, "failed to create armour hit effect", SVLogBulletCollision);
	}
	else if (effectToSpawn == EBloodEffect::BE_Blood)
	{
		if (detailsComponent->GetBloodType() == EBloodType::BT_RedBlood)
		{
			FHitResult bloodSpatterResult;
			FCollisionQueryParams queryParams;
			queryParams.AddIgnoredActors(GetActorsForBloodSpatterTraceToIgnore(bloodEffectActor));

			GetWorld()->LineTraceSingleByChannel(bloodSpatterResult, GetOwner()->GetActorLocation(),
												 GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100,
												 USvUtilities::GetEnvironmentChannel(), queryParams);

			if (bloodSpatterResult.bBlockingHit)
			{
				// spawning wall spatters as behind is a wall
				auto wallSpatterActor = GetWorld()->SpawnActor<ABloodSpatterDecal>(bloodSpatterResult.ImpactPoint, bloodSpatterResult.ImpactNormal.Rotation());
				if (!wallSpatterActor)
					UDebugMessages::LogError(this, "failed to create wall spatter", SVLogBulletCollision);
				else
					wallSpatterActor->SetupForWall();
			}
			else
			{
				GetWorld()->LineTraceSingleByChannel(bloodSpatterResult, GetOwner()->GetActorLocation(),
													 (GetOwner()->GetActorLocation() - FVector(0, 0, 200)) + GetOwner()->GetActorForwardVector() * 250,
													 USvUtilities::GetFloorTargetChannel(), queryParams);

				// spawning floor spatters
				if (bloodSpatterResult.bBlockingHit)
				{
					auto testVAlue = GetOwner()->GetActorForwardVector().Rotation().Yaw;
					auto bulletSpawnPoint = FVector(bloodEffectActor->GetActorLocation().X, bloodEffectActor->GetActorLocation().Y, bloodSpatterResult.ImpactPoint.Z);
					// auto floorSpatterActor = GetWorld()->SpawnActor<ABloodSpatterDecal>(bloodSpatterResult.ImpactPoint, bloodSpatterResult.ImpactNormal.Rotation());
					auto floorSpatterActor = GetWorld()->SpawnActor<ABloodSpatterDecal>(bulletSpawnPoint, FRotator(0, GetOwner()->GetActorForwardVector().Rotation().Yaw, 0));
					if (!floorSpatterActor)
						UDebugMessages::LogError(this, "failed to create floor spatter", SVLogBulletCollision);
					else
						floorSpatterActor->SetupForFloor(bloodEffectActor->GetActorLocation(), GetOwner()->GetActorForwardVector().Rotation(), 250);
				}
			}

			// spawn blood exit effects
			FVector entryLocation = GetOwner()->GetActorLocation();
			if (bFromSweep)
				entryLocation = sweepLoc;

			auto spatterLocation = GetLocationToSpawnBloodHitEffect(hitComponent->GetAsPrimitiveComponent(), entryLocation, GetOwner()->GetActorForwardVector());
			auto spatterActor = GetWorld()->SpawnActor<ABloodHitEffect>(spatterLocation, GetOwner()->GetActorRotation());
			if (!spatterActor)
				UDebugMessages::LogWarning(this, "failed to spawn spatter actor", SVLogBulletCollision);
			else
			{
				spatterActor->MoveBackSplatter(hitComponent->GetSpatterBackDistance());
				spatterActor->SetBloodSpatterAssetFor(bloodEffectActor);
			}

			// DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100, FColor::Green, false, 50, 0, 0);
			// DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), (GetOwner()->GetActorLocation() - FVector(0, 0, 200)) + GetOwner()->GetActorForwardVector() * 100, FColor::Green, false, 50, 0, 0);
		}
	}
}
#pragma optimize("", on)