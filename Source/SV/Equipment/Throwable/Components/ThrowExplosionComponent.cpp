// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowExplosionComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Characters/Components/DamageRecieveComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Components/EquipmentDetailsComponent.h"


// Sets default values for this component's properties
UThrowExplosionComponent::UThrowExplosionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	FuseTime = 3.0f;

	SetGenerateOverlapEvents(false);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionObjectType(USvUtilities::GetBulletCollisionObjectChannel());
	SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	OnComponentBeginOverlap.AddDynamic(this, &UThrowExplosionComponent::Overlapped);
}


// Called when the game starts
void UThrowExplosionComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UThrowExplosionComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	for (int i = 0; i < ActorsCaughtInExplosion.Num(); i++) {
		if (ActorsCaughtInExplosion[i] == OtherActor)
			return;
	}
	ActorsCaughtInExplosion.Emplace(OtherActor);
	UDebugMessages::LogDisplay(this, OtherActor->GetName() + " got caught in explosion");
	auto damageReceiveComponent = OtherActor->GetComponentByClass<UDamageRecieveComponent>();
	auto equipmentDetailsComponent = GetOwner()->GetComponentByClass<UEquipmentDetailsComponent>();
	if (damageReceiveComponent && equipmentDetailsComponent) {
		damageReceiveComponent->DoDamage(1.0f, 200, GetOwner()->GetActorLocation(), 150000);
	}
}


// Called every frame
void UThrowExplosionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UThrowExplosionComponent::FuseHandleCallback() {
	SetVisibility(false);
	bHiddenInGame = true;

	//TODO
	SetGenerateOverlapEvents(true);
	SetRelativeScale3D(FVector(ExplosionRadius));

	auto mesh = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
	if (mesh) mesh->SetVisibility(false);

	auto capsule = GetOwner()->GetComponentByClass<UCapsuleComponent>();
	if (capsule) capsule->SetSimulatePhysics(false);
}

void UThrowExplosionComponent::BeginExplosion() {
	GetOwner()->GetWorld()->GetTimerManager().SetTimer(FuseHandle, this, &UThrowExplosionComponent::FuseHandleCallback, FuseTime, false);
}
void UThrowExplosionComponent::SetFuseTime(float fuseTime) {
	FuseTime = fuseTime;
}
void UThrowExplosionComponent::SetExplosionRadius(float radius) {
	ExplosionRadius = radius;
	SetRelativeScale3D(FVector(ExplosionRadius));
}
