// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "../../../Interfaces/DamageReceiver.h"
#include "../../../Interfaces/HitComponent.h"
#include "../../../Enums/EEffectsEnum.h"
#include "BulletCollisionComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBulletCollision, Log, All);

class UCharacterDetailsComponent;

/**
 *
 */
UCLASS()
class SV_API UBulletCollisionComponent : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UBulletCollisionComponent(const FObjectInitializer &ObjectInitializer);

protected:
	UFUNCTION()
	void Overlapped(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
					int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnDestroyCallback();
	FTimerHandle DestroyTimer;

	void DisableBullet();

	UPROPERTY()
	TArray<AActor *> CollidedWith;

private:
	void AttemptToInitiateBulletSound();

	FVector GetLocationToSpawnBulletHole(UStaticMeshComponent *hitMesh);
	FVector GetLocationToSpawnBloodHitEffect(UPrimitiveComponent *hitComponent, const FVector &entryLocation, const FVector &travelDirection);

	TScriptInterface<IDamageReceiver> AttemptToGetActorsDamageReceieveComponent(AActor *otherActor);
	UCharacterDetailsComponent *AttemptToGetActorsCharacterDetailsComponent(AActor *otherActor);

	UPROPERTY()
	bool bIsDisabled;

	TArray<AActor *> GetActorsForBloodSpatterTraceToIgnore(AActor *otherActor);
	TTuple<AActor *, TScriptInterface<IHitComponent>> DetermineActorToDamageWithBullet(UPrimitiveComponent *component, AActor *otherActor);

	void GetCollisionSweepForBulletCollision(TArray<struct FHitResult> &OutHits);
	bool ActorAlreadyCollidedWith(AActor *testActor);
	void AddToAlreadyCollidedWith(AActor* actor);

	EBloodEffect GetEffectsRequiredForCollision(AActor *otherActor);

	void SpawnBloodEffects(TScriptInterface<IHitComponent> hitComponent, AActor *bloodEffectActor, UCharacterDetailsComponent *detailsComponent, bool bFromSweep, FVector sweepLoc);
};
