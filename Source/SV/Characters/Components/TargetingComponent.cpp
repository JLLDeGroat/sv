// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent.h"
#include "CharacterDetailsComponent.h"
#include "../../Interfaces/Gameplay.h"
#include "../../Interfaces/SvChar.h"
#include "../../Interfaces/HitComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Utilities/GridUtilities.h"
#include "../../GameModes/Managers/CharacterManager.h"
#include "GridMovementComponent.h"

// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FTargetData> UTargetingComponent::GetCurrentTargetData() {
	return TargetData;
}

void UTargetingComponent::DetermineTargetData() {
	TargetData.Empty();

	auto owner = GetOwner();
	auto svChar = GetOwner<ISvChar>();
	auto details = owner->GetComponentByClass<UCharacterDetailsComponent>();

	if (details) {
		TArray<TScriptInterface<ISvChar>> characters;
		ObtainPotentialTargetList(characters);

		auto possibleLocations = GetPotentialShootingLocations();

		for (int i = 0; i < characters.Num(); i++) {
			auto characterLocation = characters[i]->GetSelectableGridLocation();
			bool canTarget = false;

			for (int j = 0; j < possibleLocations.Num(); j++) {

				auto hitComponents = characters[i]->GetHitComponents();
				for (int x = 0; x < hitComponents.Num(); x++) {
					FHitResult Hit;

					owner->GetWorld()->LineTraceSingleByChannel(Hit, possibleLocations[j], hitComponents[x]->GetWorldLocation(), USvUtilities::GetEnvironmentChannel());

					if (!Hit.bBlockingHit) {
						TargetData.Emplace(FTargetData(possibleLocations[j], characters[i]));
						canTarget = true;
						break;
					}
				}

				if (canTarget) 
					break;
			}
			// owner->GetWorld()->LineTraceSingleByChannel(Hit, ownerLocation, location)
		}

		for (int i = 0; i < TargetData.Num(); i++) {
			DrawDebugLine(owner->GetWorld(), TargetData[i].GetShootLocation() + FVector(0, 0, 100), 
				TargetData[i].GetCharacter()->GetSelectableGridLocation(), FColor::Blue, true, 60, 1, 5);
		}
	}
}

bool UTargetingComponent::ObtainPotentialTargetList(TArray<TScriptInterface<ISvChar>>& FoundCharacters) const {
	auto details = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	auto targetControlType = details->GetCharacterControl() == ECharacterControl::CC_Player ?
		ECharacterControl::CC_AI :
		ECharacterControl::CC_Player;

	auto gameMode = USvUtilities::GetGameMode(GetOwner()->GetWorld());
	auto characterManager = gameMode->GetCharacterManager();

	characterManager->GetCharacterListOfCharacterType(targetControlType, FoundCharacters);

	return !FoundCharacters.IsEmpty();
}

TArray<FVector> UTargetingComponent::GetPotentialShootingLocations() {
	auto svChar = GetOwner<ISvChar>();

	auto startingLocation = svChar->GetSelectableGridLocation();

	TArray<FVector> locations;

	auto gridMovementComponent = GetOwner()->GetComponentByClass<UGridMovementComponent>();
	if (gridMovementComponent) {
		gridMovementComponent->GetMovableAdjacentTiles(svChar->GetSelectableGridLocation(), locations);
	}

	locations.Emplace(svChar->GetSelectableGridLocation());

	return locations;
}