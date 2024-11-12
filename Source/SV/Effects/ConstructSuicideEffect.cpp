// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructSuicideEffect.h"
#include "NiagaraComponent.h"
#include "../Utilities/SvUtilities.h"
#include "../Utilities/GridUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/SphereComponent.h"
#include "../Characters/Components/DamageRecieveComponent.h"


// Sets default values
AConstructSuicideEffect::AConstructSuicideEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ExplosionComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Explosion"));
	ExplosionComponent->SetupAttachment(RootComponent);
	RootComponent = ExplosionComponent;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionObjectType(USvUtilities::GetBulletCollisionObjectChannel());
	CollisionComponent->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AConstructSuicideEffect::Overlapped);
	CollisionComponent->SetSphereRadius(150);

	auto meshSystemRef = "/Script/Niagara.NiagaraSystem'/Game/Effects/Environment/ConstructSuicideExp_N.ConstructSuicideExp_N'";
	auto meshSystem = USvUtilities::GetNiagaraSystem(meshSystemRef);
	if (meshSystem)
		ExplosionComponent->SetAsset(meshSystem);


	CollisionComponent->SetVisibility(true);
	CollisionComponent->bHiddenInGame = false;
}

#pragma optimize("", off)
void AConstructSuicideEffect::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	for (int i = 0; i < ActorsCaughtInExplosion.Num(); i++) {
		if (ActorsCaughtInExplosion[i] == OtherActor)
			return;
	}

	ActorsCaughtInExplosion.Emplace(OtherActor);
	UDebugMessages::LogDisplay(this, OtherActor->GetName() + " got caught in explosion");
	auto damageReceiveComponent = OtherActor->GetComponentByClass<UDamageRecieveComponent>();
	/*auto equipmentDetailsComponent = GetOwner()->GetComponentByClass<UEquipmentDetailsComponent>();*/
	if (damageReceiveComponent) {
		damageReceiveComponent->DoDamage(1.0f, 999, 500,
			UGridUtilities::FindLookAtRotation(GetActorLocation(), OtherActor->GetActorLocation()));
	}
}
#pragma optimize("", on)