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
#include "../../Environment/Destructibles/Components/DestructibleHitComponent.h"
#include "../../Environment/Fog/Components/FogSectionComponent.h"
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
void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FTargetData> UTargetingComponent::GetCurrentTargetData()
{
	return TargetData;
}

FTargetData *UTargetingComponent::GetCurrentMainTarget()
{
	for (int i = 0; i < TargetData.Num(); i++)
		if (TargetData[i].GetId() == CurrentMainTargetId)
			return &TargetData[i];

	return nullptr;
}

void UTargetingComponent::SetCurrentMainTargetId(FGuid targetId)
{
	CurrentMainTargetId = targetId;
}

FTargetData *UTargetingComponent::GetTargetDataForActor(AActor *actor)
{
	for (int i = 0; i < TargetData.Num(); i++)
		if (TargetData[i].GetCharacter() && TargetData[i].GetCharacter()->GetAsActor() == actor)
			return &TargetData[i];

	return nullptr;
}

void UTargetingComponent::DetermineTheoreticalTargetData(FVector location)
{
	TheoreticalTargetData.Empty();

	if (location == FVector::ZeroVector)
		return UDebugMessages::LogError(this, "location is zero vector, and wont work with theoretical target data");

	auto owner = GetOwner();
	auto svChar = GetOwner<ISvChar>();
	auto details = owner->GetComponentByClass<UCharacterDetailsComponent>();

	if (details)
	{
		TArray<TScriptInterface<ISvChar>> characters;
		ObtainPotentialTargetList(characters);

		auto possibleLocations = GetPotentialShootingLocations(false, location);

		for (int i = 0; i < characters.Num(); i++)
		{
			auto characterLocation = location;

			possibleLocations.Sort([possibleLocations, characterLocation](const FVector A, const FVector B)
								   { return FVector::DistSquared(characterLocation, A) < FVector::DistSquared(characterLocation, B); });

			FTargetData foundData;
			if (GetCanTarget(characterLocation, characters[i], foundData))
			{
				TheoreticalTargetData.Emplace(foundData);
				continue;
			}

			for (int j = 0; j < possibleLocations.Num(); j++)
			{
				if (GetCanTarget(possibleLocations[j], characters[i], foundData))
				{
					TheoreticalTargetData.Emplace(foundData);
					break;
				}
			}
			// owner->GetWorld()->LineTraceSingleByChannel(Hit, ownerLocation, location)
		}
	}
}
TArray<FTargetData> UTargetingComponent::GetCurrentTheoreticalTargetData()
{
	return TheoreticalTargetData;
}

void UTargetingComponent::DetermineTargetData()
{
	TargetData.Empty();

	auto owner = GetOwner();
	auto svChar = GetOwner<ISvChar>();
	auto details = owner->GetComponentByClass<UCharacterDetailsComponent>();

	if (details)
	{
		TArray<TScriptInterface<ISvChar>> characters;
		ObtainPotentialTargetList(characters);

		auto possibleLocations = GetPotentialShootingLocations(false);

		for (int i = 0; i < characters.Num(); i++)
		{
			auto characterLocation = characters[i]->GetSelectableGridLocation();

			possibleLocations.Sort([possibleLocations, characterLocation](const FVector A, const FVector B)
								   { return FVector::DistSquared(characterLocation, A) < FVector::DistSquared(characterLocation, B); });

			auto startingLocation = svChar->GetSelectableGridLocation();
			FTargetData foundData;
			if (GetCanTarget(startingLocation, characters[i], foundData))
			{
				TargetData.Emplace(foundData);
				continue;
			}

			for (int j = 0; j < possibleLocations.Num(); j++)
			{
				if (GetCanTarget(possibleLocations[j], characters[i], foundData))
				{
					TargetData.Emplace(foundData);
					break;
				}
			}
			// owner->GetWorld()->LineTraceSingleByChannel(Hit, ownerLocation, location)
		}

		if (details->GetCharacterControl() == ECharacterControl::CC_Player)
		{
			auto hudDelegates = UHudDelegates::GetInstance();
			if (!hudDelegates)
				return UDebugMessages::LogError(this, "failed to get hud delegates, wont add targets to hud");

			hudDelegates->_ClearTargetDataHud.Broadcast();
			for (int i = 0; i < TargetData.Num(); i++)
			{
				if (i == 0)
					CurrentMainTargetId = TargetData[i].GetId();

				// DrawDebugLine(owner->GetWorld(), TargetData[i].GetShootLocation() + FVector(0, 0, 100),
				// TargetData[i].GetCharacter()->GetSelectableGridLocation(), FColor::Blue, true, 60, 1, 5);

				auto target = TargetData[i].GetCharacter()->GetAsActor();
				auto detailsComp = target->GetComponentByClass<UCharacterDetailsComponent>();
				if (!detailsComp)
				{
					UDebugMessages::LogError(this, "failed to get target component");
					continue;
				}

				hudDelegates->_AddTargetDataToHud.Broadcast(TargetData[i].GetId(), TargetData[i].GetShootLocation(),
															TargetData[i].GetCharacter()->GetSelectableGridLocation(), detailsComp->GetTargetIcon());
			}
		}
	}
}

bool UTargetingComponent::ObtainPotentialTargetList(TArray<TScriptInterface<ISvChar>> &FoundCharacters) const
{
	auto details = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	auto targetControlType = details->GetCharacterControl() == ECharacterControl::CC_Player ? ECharacterControl::CC_AI : ECharacterControl::CC_Player;

	auto gameMode = USvUtilities::GetGameMode(GetOwner()->GetWorld());
	auto characterManager = gameMode->GetCharacterManager();

	characterManager->GetCharacterListOfCharacterType(targetControlType, FoundCharacters);

	return !FoundCharacters.IsEmpty();
}

TArray<FVector> UTargetingComponent::GetPotentialShootingLocations(bool includeCurrentLocation, FVector startingLoc)
{
	auto svChar = GetOwner<ISvChar>();

	auto startingLocation = startingLoc == FVector::ZeroVector ? svChar->GetSelectableGridLocation() : startingLoc;

	TArray<FVector> locations;

	auto gridMovementComponent = GetOwner()->GetComponentByClass<UGridMovementComponent>();
	if (gridMovementComponent)
		gridMovementComponent->GetMovableAdjacentTiles(startingLocation, locations, FVector::ZeroVector, true, true, true);

	TArray<FVector> finalLocations;
	if (includeCurrentLocation)
		finalLocations.Emplace(startingLocation);

	for (int i = 0; i < locations.Num(); i++)
		finalLocations.Emplace(locations[i]);

	return finalLocations;
}

bool UTargetingComponent::GetCanTarget(FVector possibleLocation, TScriptInterface<ISvChar> character, FTargetData &foundData)
{
	auto svChar = GetOwner<ISvChar>();
	auto headLocation = svChar->GetHeadZHeight();
	auto hitComponents = character->GetHitComponents();
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(GetOwner());
	for (int x = 0; x < hitComponents.Num(); x++)
	{
		bool isValid = true;
		// DrawDebugLine(GetWorld(), possibleLocation, hitComponents[x]->GetWorldLocation(), FColor::Blue, false, 20, 0, 2);
		//  check is in fog
		TArray<FHitResult> fogHits;
		GetOwner()->GetWorld()->LineTraceMultiByChannel(fogHits, possibleLocation, hitComponents[x]->GetWorldLocation(), USvUtilities::GetFogCollisionObjectChannel(), queryParams);
		for (FHitResult fogHit : fogHits)
			if (fogHit.GetComponent()->IsA<UFogSectionComponent>())
			{
				auto fogSection = Cast<UFogSectionComponent>(fogHit.GetComponent());
				if (fogSection->GetIsInFog())
				{
					UDebugMessages::LogError(this, "found fog " + fogHit.GetActor()->GetName() + " " + fogHit.GetComponent()->GetName());
					return false;
				}
			}

		TArray<FHitResult> Hits;
		GetOwner()->GetWorld()->LineTraceMultiByChannel(Hits, possibleLocation, hitComponents[x]->GetWorldLocation(), USvUtilities::GetEnvironmentChannel());
		for (FHitResult hit : Hits)
		{
			if (hit.bBlockingHit && !hit.GetActor()->GetComponentByClass<UDestructibleHitComponent>())
			{
				UDebugMessages::LogDisplay(this, "is blocking hit, against " + hit.GetActor()->GetName());
				isValid = false;
				continue;
			}
		}

		if (isValid)
		{
			foundData = FTargetData(
				possibleLocation,
				character,
				FVector(possibleLocation.X, possibleLocation.Y, headLocation));
			return true;
		}
		// FHitResult Hit;
		// GetOwner()->GetWorld()->LineTraceSingleByChannel(Hit, possibleLocation, hitComponents[x]->GetWorldLocation(), USvUtilities::GetEnvironmentChannel());

		// if (!Hit.bBlockingHit) {
		// 	foundData = FTargetData(possibleLocation, character,
		// 		FVector(possibleLocation.X, possibleLocation.Y, headLocation));
		// 	return true;
		// }
	}
	return false;
}