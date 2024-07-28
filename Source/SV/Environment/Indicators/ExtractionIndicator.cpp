// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtractionIndicator.h"
#include "../../Utilities/SvUtilities.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/ActionsComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AExtractionIndicator::AExtractionIndicator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMeshComponent;

	BottomRingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomRightMesh"));
	BottomRingMeshComponent->SetupAttachment(RootComponent);

	TopRingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopRingMesh"));
	TopRingMeshComponent->SetupAttachment(RootComponent);

	auto baseMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/ExtractionItems/ExtractionLocationItem_MainBowl.ExtractionLocationItem_MainBowl'");
	if (baseMesh) {
		BaseMeshComponent->SetStaticMesh(baseMesh);
	}

	auto topRingMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/ExtractionItems/ExtractionLocationItem_TopRing.ExtractionLocationItem_TopRing'");
	if (topRingMesh) {
		TopRingMeshComponent->SetStaticMesh(topRingMesh);
	}

	auto bottomRingMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/ExtractionItems/ExtractionLocationItem_BottomRing.ExtractionLocationItem_BottomRing'");
	if (bottomRingMesh) {
		BottomRingMeshComponent->SetStaticMesh(bottomRingMesh);
	}

	Speed = 30;
	DefaultSpeed = Speed;

	IndicatorActivator = CreateDefaultSubobject<UBoxComponent>(TEXT("Activator"));
	IndicatorActivator->SetupAttachment(RootComponent);
	IndicatorActivator->SetBoxExtent(FVector(20, 20, 32));
	IndicatorActivator->SetRelativeLocation(FVector(0, 0, 35));

	IndicatorActivator->OnComponentBeginOverlap.AddDynamic(this, &AExtractionIndicator::Overlapped);
	IndicatorActivator->OnComponentEndOverlap.AddDynamic(this, &AExtractionIndicator::OverlapEnded);

	BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TopRingMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BottomRingMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AExtractionIndicator::BeginPlay()
{
	Super::BeginPlay();
	DeactivateIndicator();
}

void AExtractionIndicator::ActivateIndicator() {
	TopRingMeshComponent->SetVisibility(true);
	BottomRingMeshComponent->SetVisibility(true);
	TopRingMeshComponent->SetRelativeLocation(FVector(0));
	BottomRingMeshComponent->SetRelativeLocation(FVector(0));
	SetActorTickEnabled(true);
}
void AExtractionIndicator::DeactivateIndicator() {
	TopRingMeshComponent->SetVisibility(false);
	BottomRingMeshComponent->SetVisibility(false);
	TopRingMeshComponent->SetRelativeLocation(FVector(0));
	BottomRingMeshComponent->SetRelativeLocation(FVector(0));
	SetActorTickEnabled(false);
}

// Called every frame
void AExtractionIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto currentRelativeLocation = TopRingMeshComponent->GetRelativeLocation();

	auto newRelativeLocation = UKismetMathLibrary::VInterpTo_Constant(currentRelativeLocation, FVector(0, 0, 50), DeltaTime, Speed);

	TopRingMeshComponent->SetRelativeLocation(newRelativeLocation);
	BottomRingMeshComponent->SetRelativeLocation(newRelativeLocation);

	auto amountMoved = currentRelativeLocation.Z;
	auto amountToRemoveFromScale = 1.0f - (amountMoved / 50);

	TopRingMeshComponent->SetRelativeScale3D(FVector(1, 1, amountToRemoveFromScale));
	BottomRingMeshComponent->SetRelativeScale3D(FVector(1, 1, amountToRemoveFromScale));

	Speed += Speed * .05f;

	if (currentRelativeLocation.Z >= 50) {
		SetActorTickEnabled(false);
		TopRingMeshComponent->SetRelativeLocation(FVector(0));
		BottomRingMeshComponent->SetRelativeLocation(FVector(0));

		TopRingMeshComponent->SetVisibility(false);
		BottomRingMeshComponent->SetVisibility(false);

		Speed = DefaultSpeed;

		GetWorld()->GetTimerManager().SetTimer(StartTickHandle, this, &AExtractionIndicator::StartTickAgain, 1.2f);
	}
}

void AExtractionIndicator::StartTickAgain() {
	SetActorTickEnabled(true);

	TopRingMeshComponent->SetVisibility(true);
	BottomRingMeshComponent->SetVisibility(true);
}


void AExtractionIndicator::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	auto characterDetails = OtherActor->GetComponentByClass<UCharacterDetailsComponent>();
	auto actionComponent = OtherActor->GetComponentByClass<UActionsComponent>();
	if (characterDetails && actionComponent) {
		if (characterDetails->GetCharacterControl() == ECharacterControl::CC_Player &&
			OtherComp->IsA<UCapsuleComponent>()) {
			actionComponent->SetCanExtract(true);
			ActivateIndicator();
		}
	}
}

void AExtractionIndicator::OverlapEnded(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	auto characterDetails = OtherActor->GetComponentByClass<UCharacterDetailsComponent>();
	auto actionComponent = OtherActor->GetComponentByClass<UActionsComponent>();
	if (characterDetails && actionComponent) {
		if (characterDetails->GetCharacterControl() == ECharacterControl::CC_Player &&
			OtherComp->IsA<UCapsuleComponent>()) {
			actionComponent->SetCanExtract(false);
			DeactivateIndicator();
		}
	}
}