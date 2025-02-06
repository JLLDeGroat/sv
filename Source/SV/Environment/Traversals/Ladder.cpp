// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"
#include "Components/StaticMeshComponent.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Utilities/SvUtilities.h"
#include "Components/TraversalLocationsComponent.h"
#include "Components/BoxComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Navigation/NavLinkProxy.h"
#include "../../Delegates/GameplayDelegates.h"

ALadder::ALadder(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComp;

	LadderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderMesh"));
	LadderMesh->SetupAttachment(RootComponent);
	LadderMesh->SetRelativeLocation(FVector(0, 15, 0));

	LadderTop = CreateDefaultSubobject<UBoxComponent>(TEXT("LadderTop"));
	LadderBottom = CreateDefaultSubobject<UBoxComponent>(TEXT("Ladd"));
	TraversalBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Traversal"));

	LadderTop->SetupAttachment(LadderMesh);
	LadderBottom->SetupAttachment(LadderMesh);
	TraversalBox->SetupAttachment(LadderMesh);

	LadderBottom->OnComponentBeginOverlap.AddDynamic(this, &ALadder::LadderBottomOverlapped);
	LadderBottom->OnComponentEndOverlap.AddDynamic(this, &ALadder::LadderBottomEndOverlap);

	LadderTop->OnComponentBeginOverlap.AddDynamic(this, &ALadder::LadderTopOverlapped);
	LadderTop->OnComponentEndOverlap.AddDynamic(this, &ALadder::LadderTopEndOverlap);

	auto ladderMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Ladder/Ladder.Ladder'");
	if (ladderMesh) {
		LadderMesh->SetStaticMesh(ladderMesh);
		LadderMesh->SetCanEverAffectNavigation(false);
		LadderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	LadderBottom->SetBoxExtent(FVector(25, 25, 75));
	LadderBottom->SetRelativeLocation(FVector(50, 25, 75));

	LadderTop->SetBoxExtent(FVector(25, 25, 75));
	LadderTop->SetRelativeLocation(FVector(50, -25, 275));

	TraversalBox->SetBoxExtent(FVector(40, 5, 175));
	TraversalBox->SetRelativeLocation(FVector(50, 0, 175));

	LadderTop->SetVisibility(true);
	LadderTop->bHiddenInGame = false;

	LadderBottom->SetVisibility(true);
	LadderBottom->bHiddenInGame = false;

	TraversalBox->SetVisibility(true);
	TraversalBox->bHiddenInGame = false;

	TraversalBox->SetGenerateOverlapEvents(true);
	TraversalBox->SetCollisionObjectType(USvUtilities::GetTraversalObjectChannel());

	TraversalLocationsComponent = CreateDefaultSubobject<UTraversalLocationsComponent>(TEXT("LocationsComponent"));

	//NavLinkComponent = CreateDefaultSubobject< UNavLinkCustomComponent>(TEXT("CustomNavLink"));
}
void ALadder::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);


}

void ALadder::BeginPlay() {
	Super::BeginPlay();

	auto startGridLoc = UGridUtilities::GetNormalisedGridLocation(LadderBottom->GetComponentLocation());
	auto endGridLoc = UGridUtilities::GetNormalisedGridLocation(LadderTop->GetComponentLocation());

	TraversalLocationsComponent->SetStartGridLocation(startGridLoc);
	TraversalLocationsComponent->SetEndGridLocation(endGridLoc);

	auto gameplayDelegates = UGameplayDelegates::GetInstance();
	if (!gameplayDelegates)
		return UDebugMessages::LogError(this, "could not find gameplay delegates");

	FHitResult floorHit1;
	FHitResult floorHit2;

	GetWorld()->LineTraceSingleByChannel(floorHit1, startGridLoc, startGridLoc - FVector(0, 0, 500), USvUtilities::GetFloorTargetChannel());
	GetWorld()->LineTraceSingleByChannel(floorHit2, endGridLoc, endGridLoc - FVector(0, 0, 500), USvUtilities::GetFloorTargetChannel());

	if (floorHit1.bBlockingHit)
		startGridLoc.Z = floorHit1.Location.Z + 20;

	if (floorHit2.bBlockingHit)
		endGridLoc.Z = floorHit2.Location.Z + 20;

	gameplayDelegates->_OnNewLinkAdded.Broadcast(startGridLoc, endGridLoc);
}

void ALadder::LadderTopOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	CharacterAtTop = OtherActor;
	UDebugMessages::LogDisplay(this, "character at top " + OtherActor->GetName());
}

void ALadder::LadderBottomOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	CharacterAtBottom = OtherActor;
	UDebugMessages::LogDisplay(this, "character at bottom " + OtherActor->GetName());
}

void ALadder::LadderTopEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	CharacterAtTop = nullptr;
	UDebugMessages::LogDisplay(this, "character no longer at top " + OtherActor->GetName());
}

void ALadder::LadderBottomEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	CharacterAtBottom = nullptr;
	UDebugMessages::LogDisplay(this, "character no longer at bottom " + OtherActor->GetName());
}