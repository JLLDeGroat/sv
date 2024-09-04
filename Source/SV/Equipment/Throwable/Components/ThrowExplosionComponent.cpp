// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowExplosionComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Utilities/GridUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Characters/Components/DamageRecieveComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Components/EquipmentDetailsComponent.h"
#include "../Components/ThrownOwnerComponent.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../Player/Components/PawnCameraComponent.h"
#include "../../../Player/Managers/ControlManager.h"


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
		damageReceiveComponent->DoDamage(1.0f, 200, 150000, 
			UGridUtilities::FindLookAtRotation(GetOwner()->GetActorLocation(), OtherActor->GetActorLocation()));
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

	auto thrownOwner = GetOwner()->GetComponentByClass<UThrownOwnerComponent>();
	if (thrownOwner && thrownOwner->GetThrownOwner()) {
		auto details = thrownOwner->GetThrownOwner()->GetComponentByClass<UCharacterDetailsComponent>();

		if (details && details->GetCharacterControl() == ECharacterControl::CC_Player) {
			auto world = GetOwner()->GetWorld();
			auto controller = world->GetFirstPlayerController();

			auto pawnCamera = controller->GetPawn()->GetComponentByClass<UPawnCameraComponent>();

			if (!pawnCamera)
				return UDebugMessages::LogError(this, "could not find pawn camera component");

			pawnCamera->UpdateCameraState(ECameraState::CS_Control, FVector::ZeroVector, FVector::ZeroVector, true);

			auto controlManager = controller->GetComponentByClass<UControlManager>();
			if (!controlManager)
				return UDebugMessages::LogError(this, "could not get control manager");
			controlManager->SetCanMouseDesignateSelectionDecal(true);
		}
		else
			return UDebugMessages::LogError(this, "could not get character details component of thrown owner");
	}
	else
		return UDebugMessages::LogError(this, "could not get thrown owner component");
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
