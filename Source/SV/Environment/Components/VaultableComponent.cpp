// Fill out your copyright notice in the Description page of Project Settings.


#include "VaultableComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Characters/Components/VaultObstacleComponent.h"
// Sets default values for this component's properties
UVaultableComponent::UVaultableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetGenerateOverlapEvents(true);
	SetCollisionObjectType(USvUtilities::GetTriggerableChannel());

	OnComponentBeginOverlap.AddDynamic(this, &UVaultableComponent::Overlapped);
}


// Called when the game starts
void UVaultableComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVaultableComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	auto detailsComp = OtherActor->GetComponentByClass<UCharacterDetailsComponent>();
	if (OtherComp->IsA<UCapsuleComponent>() && detailsComp && detailsComp->GetCanVault()) {
		auto vaultingComponent = OtherActor->GetComponentByClass<UVaultObstacleComponent>();
		
		if (vaultingComponent) 
			vaultingComponent->ActivateVault();
	}
}