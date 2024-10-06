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
#include "../../Delegates/HudDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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

FTargetData* UTargetingComponent::GetCurrentMainTarget() {
	for (int i = 0; i < TargetData.Num(); i++)
		if (TargetData[i].GetId() == CurrentMainTargetId)
			return &TargetData[i];

	return nullptr;
}

void UTargetingComponent::SetCurrentMainTargetId(FGuid targetId) {
	CurrentMainTargetId = targetId;
}

FTargetData* UTargetingComponent::GetTargetDataForActor(AActor* actor) {
	for (int i = 0; i < TargetData.Num(); i++)
		if (TargetData[i].GetCharacter() && TargetData[i].GetCharacter()->GetAsActor() == actor)
			return &TargetData[i];

	return nullptr;
}

void UTargetingComponent::DetermineTargetData() {
	TargetData.Empty();

	auto owner = GetOwner();
	auto svChar = GetOwner<ISvChar>();
	auto details = owner->GetComponentByClass<UCharacterDetailsComponent>();

	if (details) {
		TArray<TScriptInterface<ISvChar>> characters;
		ObtainPotentialTargetList(characters);

		auto possibleLocations = GetPotentialShootingLocations(false);

		for (int i = 0; i < characters.Num(); i++) {
			auto characterLocation = characters[i]->GetSelectableGridLocation();

			possibleLocations.Sort([possibleLocations, characterLocation](const FVector A, const FVector B) {
				return FVector::DistSquared(characterLocation, A) < FVector::DistSquared(characterLocation, B);
				});

			auto startingLocation = svChar->GetSelectableGridLocation();
			if (GetCanTarget(startingLocation, characters[i]))
				continue;

			for (int j = 0; j < possibleLocations.Num(); j++) {
				if (GetCanTarget(possibleLocations[j], characters[i]))
					break;
			}
			// owner->GetWorld()->LineTraceSingleByChannel(Hit, ownerLocation, location)
		}

		if (details->GetCharacterControl() == ECharacterControl::CC_Player) {
			auto hudDelegates = UHudDelegates::GetInstance();
			if (!hudDelegates)
				return UDebugMessages::LogError(this, "failed to get hud delegates, wont add targets to hud");

			hudDelegates->_ClearTargetDataHud.Broadcast();
			for (int i = 0; i < TargetData.Num(); i++) {
				if (i == 0)
					CurrentMainTargetId = TargetData[i].GetId();

				//DrawDebugLine(owner->GetWorld(), TargetData[i].GetShootLocation() + FVector(0, 0, 100),
					//TargetData[i].GetCharacter()->GetSelectableGridLocation(), FColor::Blue, true, 60, 1, 5);

				hudDelegates->_AddTargetDataToHud.Broadcast(TargetData[i].GetId(), TargetData[i].GetShootLocation(),
					TargetData[i].GetCharacter()->GetSelectableGridLocation());
			}
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

TArray<FVector> UTargetingComponent::GetPotentialShootingLocations(bool includeCurrentLocation) {
	auto svChar = GetOwner<ISvChar>();

	auto startingLocation = svChar->GetSelectableGridLocation();
	TArray<FVector> locations;

	auto gridMovementComponent = GetOwner()->GetComponentByClass<UGridMovementComponent>();
	if (gridMovementComponent)
		gridMovementComponent->GetMovableAdjacentTiles(startingLocation, locations, FVector::ZeroVector, true);

	TArray<FVector> finalLocations;
	if (includeCurrentLocation)
		finalLocations.Emplace(startingLocation);

	for (int i = 0; i < locations.Num(); i++)
		finalLocations.Emplace(locations[i]);

	return finalLocations;
}

bool UTargetingComponent::GetCanTarget(FVector possibleLocation, TScriptInterface<ISvChar> character) {
	auto svChar = GetOwner<ISvChar>();
	auto headLocation = svChar->GetHeadZHeight();
	auto hitComponents = character->GetHitComponents();
	for (int x = 0; x < hitComponents.Num(); x++) {
		FHitResult Hit;
		GetOwner()->GetWorld()->LineTraceSingleByChannel(Hit, possibleLocation, hitComponents[x]->GetWorldLocation(), USvUtilities::GetEnvironmentChannel());

		if (!Hit.bBlockingHit) {
			TargetData.Emplace(FTargetData(possibleLocation, character,
				FVector(possibleLocation.X, possibleLocation.Y, headLocation)));
			return true;
		}
	}
	return false;
}