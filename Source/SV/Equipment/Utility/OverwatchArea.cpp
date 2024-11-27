// Fill out your copyright notice in the Description page of Project Settings.


#include "OverwatchArea.h"
#include "../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Characters/Components/HitBoxComponent.h"
#include "../../Characters/Components/TargetingComponent.h"
#include "../../Characters/Components/HitCapsuleComponent.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Characters/Components/AttackComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Equipment/Equipment.h"
#include "Components/StaticMeshComponent.h"
#include "../../Player/Actions/TargetAction.h"
#include "../../Player/Components/PawnCameraComponent.h"
#include "../../Delegates/GameplayDelegates.h"
#include "../../GameModes/Managers/OverwatchManager.h"
#include "../../Utilities/SvUtilities.h"
// Sets default values
AOverwatchArea::AOverwatchArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = MeshComponent;

	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Utilities/OverwatchArea.OverwatchArea'");
	if (mesh)
		MeshComponent->SetStaticMesh(mesh);

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AOverwatchArea::OnOverlapped);

	bIsActive = false;
	bCanActivate = false;

	auto gamePlayDelegates = UGameplayDelegates::GetInstance();
	if (gamePlayDelegates) {
		gamePlayDelegates->_ActivateOverwatchActors.AddDynamic(this, &AOverwatchArea::OnCanActivateRequest);
		gamePlayDelegates->_RemoveUnusedOverwatchActors.AddDynamic(this, &AOverwatchArea::OnRemoveRequest);
	}
}

void AOverwatchArea::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);


}

void AOverwatchArea::SetupForCollision() {
	if (!MeshComponent)
		return UDebugMessages::LogError(this, "failed to get mesh component");

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionObjectType(USvUtilities::GetBulletCollisionObjectChannel());
	MeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	MeshComponent->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionChannel(), ECR_Overlap);
	MeshComponent->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	MeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	MeshComponent->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//MeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	UDebugMessages::LogDisplay(this, "Setup for collision");

}
void AOverwatchArea::OnOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	UDebugMessages::LogDisplay(this, "overlappped");
	if (bIsActive || !bCanActivate) return UDebugMessages::LogDisplay(this, "already overlapped or cannot activate");

	if (!OverwatchOwner)
		return UDebugMessages::LogError(this, "could not find overwatch owner");

	if (OtherComp->IsA<UBoxComponent>() || OtherComp->IsA<UCapsuleComponent>()) {
		auto characterDetails = OverwatchOwner->GetComponentByClass<UCharacterDetailsComponent>();
		auto otherDetails = OtherActor->GetComponentByClass<UCharacterDetailsComponent>();

		if (characterDetails && otherDetails && characterDetails->GetCharacterControl() != otherDetails->GetCharacterControl()) {

			bIsActive = true;

			auto overwatchManager = USvUtilities::GetGameModeOverwatchManager(GetWorld());

			if (!overwatchManager)
				return UDebugMessages::LogError(this, "cannot do overwatch, there is no overwatch manager");

			overwatchManager->AddToOverwatchPotentials(OverwatchOwner, OtherActor, OtherComp);
			Destroy();
		}
	}
}
void AOverwatchArea::SetOverWatchOwner(AActor* actor) {
	OverwatchOwner = actor;
}

FVector AOverwatchArea::GetOverwatchOwnerLocation() {
	if (OverwatchOwner) return OverwatchOwner->GetActorLocation();
	return FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AOverwatchArea::BeginPlay()
{
	Super::BeginPlay();

}


void AOverwatchArea::OnCanActivateRequest(ECharacterControl control) {
	if (!OverwatchOwner) return UDebugMessages::LogError(this, "cannot find overwatch owner OnCanActivateRequest");
	SetupForCollision();
	auto details = OverwatchOwner->GetComponentByClass<UCharacterDetailsComponent>();
	if (details->GetCharacterControl() == control)
		bCanActivate = true;
}


void AOverwatchArea::OnRemoveRequest(ECharacterControl control) {
	if (!OverwatchOwner) return UDebugMessages::LogError(this, "cannot find overwatch owner OnRemoveRequest");

	auto details = OverwatchOwner->GetComponentByClass<UCharacterDetailsComponent>();
	if (details->GetCharacterControl() == control)
		Destroy();
}
