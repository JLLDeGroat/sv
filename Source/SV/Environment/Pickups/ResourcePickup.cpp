// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourcePickup.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Characters/Components/ActionsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Characters/Components/PickupResourceComponent.h"
#include "../Components/Pickup/PickupDetailsComponent.h"

#include "../../Utilities/SvUtilities.h"

AResourcePickup::AResourcePickup(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {


	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup"));
	RootComponent = PickupMeshComponent;

	PickupMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	PickupMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AResourcePickup::Overlapped);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AResourcePickup::EndOverlap);
	BoxComponent->SetGenerateOverlapEvents(true);

	PickupMeshComponent->SetWorldScale3D(FVector(.5f));
	BoxComponent->SetBoxExtent(FVector(200));

	PickupDetailsComponent = CreateDefaultSubobject<UPickupDetailsComponent>(TEXT("PickupDetails"));
}

void AResourcePickup::BeginPlay() {
	Super::BeginPlay();

	PickupMeshComponent->SetSimulatePhysics(true);
}

void AResourcePickup::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Pickups/DroppedResource.DroppedResource'");
	if (mesh) {
		PickupMeshComponent->SetStaticMesh(mesh);
	}
}

void AResourcePickup::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherComp->IsA<UCapsuleComponent>()) {
		auto pickupComponent = OtherActor->GetComponentByClass<UPickupResourceComponent>();

		if (!pickupComponent)
			return UDebugMessages::LogError(this, OtherActor->GetName() + " does not have pickup component");


		pickupComponent->AddToPickupActors(this);
	}
}

void AResourcePickup::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherComp->IsA<UCapsuleComponent>()) {
		auto pickupComponent = OtherActor->GetComponentByClass<UPickupResourceComponent>();
		if (pickupComponent) {
			pickupComponent->RemovePickupActor(this);
		}
	}
}