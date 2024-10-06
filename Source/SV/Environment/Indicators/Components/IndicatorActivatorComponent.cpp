// Fill out your copyright notice in the Description page of Project Settings.


#include "IndicatorActivatorComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../Characters/Components/PickupResourceComponent.h"
#include "../../../Characters/Components/ActionsComponent.h"
#include "../../../Environment/Indicators/Components/IndicatorLinkComponent.h"
#include "Components/CapsuleComponent.h"
#include "../PickupIndicator.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
// Sets default values
UIndicatorActivatorComponent::UIndicatorActivatorComponent(const FObjectInitializer& ObjectInitializer)
	: UBoxComponent(ObjectInitializer)
{
	BottomRingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomRightMesh"));
	TopRingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopRingMesh"));

	if (GetOwner() && GetOwner()->GetRootComponent()) {
		TopRingMeshComponent->SetupAttachment(GetOwner()->GetRootComponent());
		BottomRingMeshComponent->SetupAttachment(GetOwner()->GetRootComponent());
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

	OnComponentBeginOverlap.AddDynamic(this, &UIndicatorActivatorComponent::Overlapped);
	OnComponentEndOverlap.AddDynamic(this, &UIndicatorActivatorComponent::OverlapEnded);

	TopRingMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BottomRingMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsActivated = false;
	PrimaryComponentTick.bCanEverTick = true;

	auto material = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/ExtractionIndicator_M.ExtractionIndicator_M'");
	IndicatorMaterial = UMaterialInstanceDynamic::Create(material, this);
	TopRingMeshComponent->SetMaterial(0, IndicatorMaterial);
	BottomRingMeshComponent->SetMaterial(0, IndicatorMaterial);
}

void UIndicatorActivatorComponent::SetMaterialColour(FLinearColor color) {
	IndicatorMaterial->SetVectorParameterValue("IndicatorColour", color);
}


void UIndicatorActivatorComponent::BeginPlay() {
	Super::BeginPlay();
	DeactivateIndicator(nullptr);
}

void UIndicatorActivatorComponent::SetIndicatorType(EIndicatorType indicatorType) {
	IndicatorType = indicatorType;
}

void UIndicatorActivatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
		SetComponentTickEnabled(false);
		TopRingMeshComponent->SetRelativeLocation(FVector(0));
		BottomRingMeshComponent->SetRelativeLocation(FVector(0));

		TopRingMeshComponent->SetVisibility(false);
		BottomRingMeshComponent->SetVisibility(false);

		Speed = DefaultSpeed;

		GetWorld()->GetTimerManager().SetTimer(StartTickHandle, this, &UIndicatorActivatorComponent::StartTickAgain, 1.2f);
	}
}

void UIndicatorActivatorComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	auto characterDetails = OtherActor->GetComponentByClass<UCharacterDetailsComponent>();
	if (characterDetails && characterDetails->GetCharacterControl() == ECharacterControl::CC_Player &&
		OtherComp->IsA<UCapsuleComponent>()) {
		ActivateIndicator(OtherActor);
	}
}

void UIndicatorActivatorComponent::OverlapEnded(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	auto characterDetails = OtherActor->GetComponentByClass<UCharacterDetailsComponent>();
	auto actionComponent = OtherActor->GetComponentByClass<UActionsComponent>();
	if (characterDetails && characterDetails->GetCharacterControl() == ECharacterControl::CC_Player &&
		OtherComp->IsA<UCapsuleComponent>()) {
		DeactivateIndicator(OtherActor);
		UDebugMessages::LogDisplay(this, "stop indicating");
	}
}

void UIndicatorActivatorComponent::StartTickAgain() {
	if (bIsActivated) {
		SetComponentTickEnabled(true);

		TopRingMeshComponent->SetVisibility(true);
		BottomRingMeshComponent->SetVisibility(true);
	}
}


void UIndicatorActivatorComponent::ActivateIndicator(AActor* activateOn) {
	TopRingMeshComponent->SetVisibility(true);
	BottomRingMeshComponent->SetVisibility(true);
	TopRingMeshComponent->SetRelativeLocation(FVector(0));
	BottomRingMeshComponent->SetRelativeLocation(FVector(0));
	SetComponentTickEnabled(true);
	bIsActivated = true;
	if (activateOn) {
		switch (IndicatorType) {
		case EIndicatorType::IT_Extract:
		{
			auto actionComponent = activateOn->GetComponentByClass<UActionsComponent>();
			actionComponent->SetCanExtract(true);
		}
		break;
		case EIndicatorType::IT_Interact:
		{
			UDebugMessages::LogError(this, "INTERACTING WITH IT_INTERACT");
			auto pickupComponent = activateOn->GetComponentByClass<UPickupResourceComponent>();
			if (pickupComponent) {
				auto linkComponent = GetOwner()->GetComponentByClass<UIndicatorLinkComponent>();
				pickupComponent->AddToPickupActors(linkComponent->GetPickupIndicatingTo());
			}
		}
		break;
		default:
		{
			UDebugMessages::LogError(this, "Failed to get any indicator type");
		}break;
		}
	}
}
void UIndicatorActivatorComponent::DeactivateIndicator(AActor* activateOn) {
	TopRingMeshComponent->SetVisibility(false);
	BottomRingMeshComponent->SetVisibility(false);
	TopRingMeshComponent->SetRelativeLocation(FVector(0));
	BottomRingMeshComponent->SetRelativeLocation(FVector(0));
	SetComponentTickEnabled(false);
	bIsActivated = false;

	if (activateOn) {
		switch (IndicatorType) {
		case EIndicatorType::IT_Extract:
		{
			auto actionComponent = activateOn->GetComponentByClass<UActionsComponent>();
			actionComponent->SetCanExtract(false);
		}
		break;
		case EIndicatorType::IT_Interact:
		{
			UDebugMessages::LogError(this, "EIndicatorType::IT_Interact TODO");
		}
		break;
		default:
		{
			UDebugMessages::LogError(this, "Failed to get any indicator type");
		}break;
		}
	}
}