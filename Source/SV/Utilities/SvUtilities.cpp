// Fill out your copyright notice in the Description page of Project Settings.


#include "SvUtilities.h"
#include "../GameModes/GameplayMode.h"
#include "../GameModes/Managers/CharacterManager.h"
#include "../GameModes/Managers/OverwatchManager.h"
#include "../Interfaces/Gameplay.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Instance/SvGameInstance.h"
#include "../Instance/Managers/CurrentGameDataManager.h"
#include "NiagaraSystem.h"
#include "../Player/Managers/SelectionManager.h"
#include "../Interfaces/Selectable.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GeometryCache.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

#include "../Characters/Components/CharacterDetailsComponent.h"
#include "../Environment/Components/VaultableComponent.h"

ECollisionChannel USvUtilities::GetFloorTargetChannel() {
	return ECollisionChannel::ECC_GameTraceChannel1;
}
ECollisionChannel USvUtilities::GetClickableChannel() {
	return ECollisionChannel::ECC_GameTraceChannel2;
}
ECollisionChannel USvUtilities::GetClickableEnvironmentChannel() {
	return ECollisionChannel::ECC_GameTraceChannel8;
}
ECollisionChannel USvUtilities::GetEnvironmentChannel() {
	return ECollisionChannel::ECC_GameTraceChannel3;
}
ECollisionChannel USvUtilities::GetBulletCollisionChannel() {
	return ECollisionChannel::ECC_GameTraceChannel4;
}

ECollisionChannel USvUtilities::GetBulletCollisionObjectChannel() {
	return ECC_GameTraceChannel5;
}

ECollisionChannel USvUtilities::GetWorldSelectChannel() {
	return ECC_GameTraceChannel6;
}

ECollisionChannel USvUtilities::GetTriggerableChannel() {
	return ECC_GameTraceChannel7;
}

ECollisionChannel USvUtilities::GetFogCollisionObjectChannel() {
	return ECC_GameTraceChannel9;
}

int USvUtilities::FormatLocation(float val) {
	int value = val;

	if (value == 0) return 0;

	auto left = (int)val % 50;

	auto addition = left > 25 ?
		-(50 - left) :
		left;

	return value - addition;
}

UStaticMesh* USvUtilities::GetStaticMesh(FString reference) {
	return Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

USkeletalMesh* USvUtilities::GetSkeletonMesh(FString reference) {
	return Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

UNiagaraSystem* USvUtilities::GetNiagaraSystem(FString reference) {
	return Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

UMaterial* USvUtilities::GetMaterial(FString reference) {
	return Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

UClass* USvUtilities::GetClass(FString reference) {
	return Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

UTexture2D* USvUtilities::GetTexture(FString reference) {
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

UTextureRenderTarget2D* USvUtilities::GetRenderTarget2D(int targetNumber) {
	FString targetNumberAsString = FString::SanitizeFloat(targetNumber, 0);
	FString reference = "/Script/Engine.TextureRenderTarget2D'/Game/RenderTargets/RenderTarget" + targetNumberAsString + "_RT.RenderTarget" + targetNumberAsString + "_RT'";
	//FString reference = "/Script/Engine.TextureRenderTarget2D'/Game/RenderTargets/RenderTarget1_RT.RenderTarget1_RT'";

	FSoftObjectPath RenderTargetPath(reference);
	UTextureRenderTarget2D* LoadedRenderTarget = Cast<UTextureRenderTarget2D>(RenderTargetPath.TryLoad());
	return LoadedRenderTarget;
	//return Cast<UTextureRenderTarget2D>(StaticLoadObject(UTextureRenderTarget2D::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

float USvUtilities::GetGridGape() { return 100.0f; }

TScriptInterface<IGameplay> USvUtilities::GetGameMode(UWorld* world) {
	return Cast<AGameplayMode>(world->GetAuthGameMode());
}

UCharacterManager* USvUtilities::GetGameModeCharacterManager(UWorld* world) {
	auto gameMode = GetGameMode(world);
	return gameMode->GetCharacterManager();
}
UObjectivesManager* USvUtilities::GetGameModeObjectiveManager(UWorld* world) {
	auto gameMode = GetGameMode(world);
	return gameMode->GetObjectivesManager();
}
UOverwatchManager* USvUtilities::GetGameModeOverwatchManager(UWorld* world) {
	auto gameMode = GetGameMode(world);
	return gameMode->GetOverwatchManager();
}

void USvUtilities::GetAdjacentGridTiles(FVector location, TArray<FVector>& adjacentTiles) {
	auto gridGape = GetGridGape();

	if (IsInBounds(location + FVector(gridGape, 0, 0)))
		adjacentTiles.Emplace(location + FVector(gridGape, 0, 0));

	if (IsInBounds(location + FVector(-gridGape, 0, 0)))
		adjacentTiles.Emplace(location + FVector(-gridGape, 0, 0));

	if (IsInBounds(location + FVector(0, gridGape, 0)))
		adjacentTiles.Emplace(location + FVector(0, gridGape, 0));

	if (IsInBounds(location + FVector(0, -gridGape, 0)))
		adjacentTiles.Emplace(location + FVector(0, -gridGape, 0));
}

bool USvUtilities::IsInBounds(FVector location) {
	if (location.X > 10000 || location.X < -1000 ||
		location.Y > 10000 || location.Y < -1000)
		return false;

	return true;
}

bool USvUtilities::AreGridLocationsAdjacent(FVector loc1, FVector loc2) {

	if (GetTileElevation(loc1) == GetTileElevation(loc2)) {

		TArray<FVector> adjacentTiles;
		GetAdjacentGridTiles(loc1, adjacentTiles);

		for (int i = 0; i < adjacentTiles.Num(); i++) {
			if (adjacentTiles[i].X == loc2.X &&
				adjacentTiles[i].Y == loc2.Y)
				return true;
		}
	}
	return false;
}

TArray<FVector> USvUtilities::OrderByClosestTo(FVector location, TArray<FVector> unorderedLocations) {
	TArray<FVector> adjacentTiles;

	if (location != FVector::ZeroVector) {
		adjacentTiles.Empty();
		while (unorderedLocations.Num() > 0) {
			float minDistance = -1;
			FVector loc = FVector::ZeroVector;
			auto index = -1;
			for (int i = 0; i < unorderedLocations.Num(); i++) {
				auto distance = FVector::Dist(unorderedLocations[i], location);
				//UDebugMessages::LogDisplay(this, "distance between " + location.ToString() + " and " + unorderedLocations[i].ToString() + " was " + FString::SanitizeFloat(distance, 2));
				if (loc == FVector::ZeroVector || distance < minDistance) {
					minDistance = distance;
					loc = unorderedLocations[i];
					index = i;
				}
			}

			adjacentTiles.Emplace(unorderedLocations[index]);
			unorderedLocations.RemoveAt(index);
		}
	}

	return adjacentTiles;
}

int USvUtilities::GetTileElevation(FVector loc) {
	int val = 1;
	if (loc.Z > 200) val = 2;
	else if (loc.Z > 400) val = 3;
	else if (loc.Z > 600) val = 4;
	else if (loc.Z > 800) val = 5;

	return val;
}

USvGameInstance* USvUtilities::GetGameInstance(UWorld* world) {
	return world->GetGameInstance<USvGameInstance>();
}
FCurrentGameData* USvUtilities::GetCurrentGameData(UWorld* world) {
	auto instance = GetGameInstance(world);
	if (!instance || !instance->GetCurrentGameDataManager() || !instance->GetCurrentGameDataManager()->GetCurrentGameData())
		return nullptr;

	return instance->GetCurrentGameDataManager()->GetCurrentGameData();
}

TArray<FVector> USvUtilities::RandomizeList(TArray<FVector> list, FRandomStream stream) {
	TArray<FVector> result = list;
	if (result.Num() > 0)
	{
		int32 LastIndex = result.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = stream.RandRange(i, LastIndex);
			if (i != Index)
				result.Swap(i, Index);
		}
	}
	return result;
}

TArray<FVector> USvUtilities::RandomizeList(TArray<FVector> list) {
	TArray<FVector> result = list;
	if (result.Num() > 0)
	{
		int32 LastIndex = result.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
				result.Swap(i, Index);
		}
	}
	return result;
}

int USvUtilities::GetWorldMapGridMultiplier() {
	return 100;
}

FVector USvUtilities::DetermineAccuracyInidicatorScale(FVector source, FVector target, float accuracy, float accuracyDecay, float baseAccuracy, float distanceModifier) {
	auto gunAccuracy = (FVector::Dist(source, target) * distanceModifier) - accuracy;
	if (gunAccuracy < 1) gunAccuracy = 1;
	gunAccuracy = gunAccuracy * accuracyDecay;
	return FVector(gunAccuracy / 100) + FVector(baseAccuracy);
}

void USvUtilities::AttemptToStartWinLossChecker(UWorld* world) {
	auto gameMode = GetGameMode(world);
	gameMode->AttemptToStartWinLossChecker();
}

AActor* USvUtilities::AttemptToGetCurrentSelectedActor(UWorld* world) {
	auto controller = world->GetFirstPlayerController();
	auto selectionManager = controller->GetComponentByClass<USelectionManager>();

	if (selectionManager && selectionManager->GetSelected())
		return selectionManager->GetSelected()->GetAsActor();


	return nullptr;
}

void USvUtilities::AttemptToStartStatUpdater(AActor* statOwner, EStatisticType statType, float value) {
	auto gameMode = GetGameMode(statOwner->GetWorld());
	gameMode->StartStatRunnable(statOwner, statType, value);
}

FString USvUtilities::GetSocketNameFromAttachment(EAttachType attachmentType) {
	FString result = "invalid";
	switch (attachmentType) {
	case EAttachType::AT_RightHolster:
		result = "SecondaryHolsterSocket";
		break;
	case EAttachType::AT_Backpack:
		result = "BackPackSocket";
		break;
	case EAttachType::AT_RightHand:
		result = "RightHandSocket";
		break;

	case EAttachType::AT_RightArm:
		result = "RightForeArmSocket";
		break;
	case EAttachType::AT_LeftArm:
		result = "LeftForeArmSocket";
		break;
	case EAttachType::AT_LeftHand:
		result = "LeftHandSocket";
		break;
	}

	return result;
}

UMaterial* USvUtilities::GetBulletHoleMaterial() {
	return GetMaterial("/Script/Engine.Material'/Game/Decals/BulletHoles/BulletHole1_M.BulletHole1_M'");
}

UGeometryCache* USvUtilities::GetGeometryCache(FString reference) {
	return Cast<UGeometryCache>(StaticLoadObject(UGeometryCache::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

UGeometryCache* USvUtilities::GetRandomBloodSpatterGeoCache() {

	int random = FMath::RandRange(1, 5);
	FString reference = "/Script/GeometryCache.GeometryCache'/Game/Effects/Spatter/Spatter7.Spatter7'";

	switch (random) {
	case 1:
	case 2:
		reference = "/Script/GeometryCache.GeometryCache'/Game/Effects/Spatter/Spatter8.Spatter8'";
	case 3:
	case 4:
		reference = "/Script/GeometryCache.GeometryCache'/Game/Effects/Spatter/Spatter9.Spatter9'";
	}

	return GetGeometryCache(reference);
}

UGeometryCollection* USvUtilities::GetGeometryCollection(FString reference) {
	return Cast<UGeometryCollection>(StaticLoadObject(UGeometryCollection::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

UMaterial* USvUtilities::GetRandomBloodSpatterForWall() {
	auto reference = "/Script/Engine.Material'/Game/Decals/Spatters/Wall/WallSpatter10_M.WallSpatter10_M'";

	auto random = FMath::RandRange(0, 9);
	switch (random) {
	case 1:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Wall/WallSpatter1_M.WallSpatter1_M'";
		break;
	case 2:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Wall/WallSpatter2_M.WallSpatter2_M'";
		break;
	case 3:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Wall/WallSpatter3_M.WallSpatter3_M'";
		break;
	case 4:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Wall/WallSpatter4_M.WallSpatter4_M'";
		break;
	case 5:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Wall/WallSpatter5_M.WallSpatter5_M'";
		break;
	case 6:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Wall/WallSpatter6_M.WallSpatter6_M'";
		break;
	case 7:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Wall/WallSpatter7_M.WallSpatter7_M'";
		break;
	case 8:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Wall/WallSpatter8_M.WallSpatter8_M'";
		break;
	case 9:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Wall/WallSpatter9_M.WallSpatter9_M'";
		break;
	}

	return GetMaterial(reference);
}
UMaterial* USvUtilities::GetRandomBloodSpatterForFloor() {
	auto reference = "/Script/Engine.Material'/Game/Decals/Spatters/Floor/FloorSpatter1_M.FloorSpatter1_M'";
	auto random = FMath::RandRange(0, 7);
	switch (random) {
	case 1:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Floor/FloorSpatter2_M.FloorSpatter2_M'";
		break;
	case 2:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Floor/FloorSpatter3_M.FloorSpatter3_M'";
		break;
	case 3:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Floor/FloorSpatter4_M.FloorSpatter4_M'";
		break;
	case 4:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Floor/FloorSpatter5_M.FloorSpatter5_M'";
		break;
	case 5:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Floor/FloorSpatter6_M.FloorSpatter6_M'";
		break;
	case 6:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Floor/FloorSpatter7_M.FloorSpatter7_M'";
		break;
	case 7:
		reference = "/Script/Engine.Material'/Game/Decals/Spatters/Floor/FloorSpatter8_M.FloorSpatter8_M'";
		break;
	}
	return GetMaterial(reference);
}


float USvUtilities::GetNoFogValue() {
	return 0.01f;
}
float USvUtilities::GetFogValue() {
	return 1;
}
float USvUtilities::GetFullFogValue() {
	return 5;
}
FString USvUtilities::GetFogVariableName() {
	return "FogAmount";
}

void USvUtilities::GetAdjacentTilesForFogCalculation(AActor* startActor, TArray<FVector>& validAdjacentTiles) {
	TArray<FVector> adjacentTiles;
	GetAdjacentGridTiles(startActor->GetActorLocation(), adjacentTiles);

	bool bIgnoreVaultables = true; // fog calc does not care about vaultables

	for (int i = 0; i < adjacentTiles.Num(); i++) {
		FHitResult EnvironmentHit;
		startActor->GetWorld()->LineTraceSingleByChannel(EnvironmentHit, startActor->GetActorLocation(), adjacentTiles[i], USvUtilities::GetEnvironmentChannel());

		FHitResult EntityHit;
		FCollisionQueryParams EntityHitParams;
		EntityHitParams.AddIgnoredActor(startActor);
		startActor->GetWorld()->LineTraceSingleByChannel(EntityHit, startActor->GetActorLocation(), adjacentTiles[i], USvUtilities::GetClickableChannel(), EntityHitParams);


		auto detailsComponent = startActor->GetComponentByClass<UCharacterDetailsComponent>();
		auto hasVaultComponentAndCanVault = EnvironmentHit.GetActor() && EnvironmentHit.GetActor()->GetComponentByClass<UVaultableComponent>()
			&& detailsComponent && detailsComponent->GetCanVault();

		if ((!EnvironmentHit.bBlockingHit && !EntityHit.bBlockingHit) ||
			(EnvironmentHit.bBlockingHit && hasVaultComponentAndCanVault && !bIgnoreVaultables))
		{
			validAdjacentTiles.Emplace(adjacentTiles[i]);
		}
		//else UDebugMessages::LogError(this, "Invalid spot " + adjacentTiles[i].ToString());
	}
}