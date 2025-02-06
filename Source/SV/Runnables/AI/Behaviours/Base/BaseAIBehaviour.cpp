// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIBehaviour.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../Interfaces/SvChar.h"
#include "../../../../Delegates/AIDelegates.h"
#include "../../../../World/WorldGridItemActor.h"
#include "../Components/AiMovementComponent.h"
#include "../../../../Utilities/SvUtilities.h"
#include "../../../../Utilities/GridUtilities.h"
#include "../../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../../Environment/Fog/Components/FogSectionComponent.h"

UBaseAIBehaviour::UBaseAIBehaviour(const FObjectInitializer& ObjectInitializer)
	: UObject(ObjectInitializer) {
	CompletedBehaviour = false;

	auto aiDelegates = UAIDelegates::GetInstance();
	if (aiDelegates)
		aiDelegates->_AICharacterFinishedBehaviour.AddDynamic(this, &UBaseAIBehaviour::SetCompletedBehaviour);
	else
		UDebugMessages::LogError(this, "failed to bind to delegate");

	_randomStream = FRandomStream(123);

	MovementComponent = CreateDefaultSubobject<UAiMovementComponent>(TEXT("Movement"));
}


void UBaseAIBehaviour::DoBehaviour() {
	UDebugMessages::LogError(this, "behaviour not coded, error");
}

void UBaseAIBehaviour::SetEnemyAndCharacters(AActor* enemy, TArray<TScriptInterface<ISvChar>> allCharacters) {
	AllCharacters = allCharacters;
	ThisEnemy = enemy;
}

void UBaseAIBehaviour::SetBehaviourTargets(TArray<TScriptInterface<ISvChar>> targets) {
	BehaviourTargets = targets;
}
void UBaseAIBehaviour::SetBehaviourTarget(TScriptInterface<ISvChar> targets) {
	BehaviourTargets.Emplace(targets);
}

AActor* UBaseAIBehaviour::GetThisEnemy() {
	return ThisEnemy;
}
TArray<TScriptInterface<ISvChar>> UBaseAIBehaviour::GetAllCharacters() {
	return AllCharacters;
}
TArray<TScriptInterface<ISvChar>> UBaseAIBehaviour::GetBehaviourTargets() {
	return BehaviourTargets;
}

bool UBaseAIBehaviour::GetCompletedBehaviour() {
	return CompletedBehaviour;
}
bool UBaseAIBehaviour::GetCompletedBehaviourAndWaitIfNot(float seconds) {
	if (!CompletedBehaviour)
		FPlatformProcess::Sleep(seconds);

	return CompletedBehaviour;
}
void UBaseAIBehaviour::SetCompletedBehaviour() {
	CompletedBehaviour = true;
	UDebugMessages::LogDisplay(this, "Behaviour set to complete");

	ClearInternalFlags(EInternalObjectFlags::Async);

	MovementComponent->ClearInternalFlags(EInternalObjectFlags::Async);

	auto aiDelegates = UAIDelegates::GetInstance();
	if (aiDelegates)
		aiDelegates->_AICharacterFinishedBehaviour.RemoveDynamic(this, &UBaseAIBehaviour::SetCompletedBehaviour);
}

void UBaseAIBehaviour::SpawnDebugGrid_SetIsStart(FVector location, float delay) {
	auto world = GetWorld();
	FPlatformProcess::Sleep(.001f);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location, delay] {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsStart();
		actor->SetAutoDestroy(delay);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseAIBehaviour::SpawnDebugGrid_SetIsEnd(FVector location, float delay) {
	auto world = GetWorld();
	FPlatformProcess::Sleep(.001f);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location, delay] {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsEnd();
		actor->SetAutoDestroy(delay);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseAIBehaviour::SpawnDebugGrid_SetIsOffshoot(FVector location, float delay) {
	auto world = GetWorld();
	FPlatformProcess::Sleep(.001f);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location, delay] {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsOffshoot();
		actor->SetAutoDestroy(delay);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseAIBehaviour::SpawnDebugGrid_SetIsSpawn(FVector location, float delay) {
	auto world = GetWorld();
	FPlatformProcess::Sleep(.001f);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location, delay] {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsSpawn();
		actor->SetAutoDestroy(delay);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseAIBehaviour::SpawnDebugGrid_SetIsObstacle(FVector location, float delay) {
	auto world = GetWorld();
	FPlatformProcess::Sleep(.001f);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location, delay] {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsObstacle();
		actor->SetAutoDestroy(delay);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void UBaseAIBehaviour::SpawnDebugGrid_SetIsStart(TArray<FVector> locations, FVector offset, float delay) {
	for (int i = 0; i < locations.Num(); i++)
		SpawnDebugGrid_SetIsStart(locations[i] + offset, delay);
}
void UBaseAIBehaviour::SpawnDebugGrid_SetIsEnd(TArray<FVector> locations, FVector offset, float delay) {
	for (int i = 0; i < locations.Num(); i++)
		SpawnDebugGrid_SetIsEnd(locations[i] + offset, delay);
}
void UBaseAIBehaviour::SpawnDebugGrid_SetIsOffshoot(TArray<FVector> locations, FVector offset, float delay) {
	for (int i = 0; i < locations.Num(); i++)
		SpawnDebugGrid_SetIsOffshoot(locations[i] + offset, delay);
}
void UBaseAIBehaviour::SpawnDebugGrid_SetIsSpawn(TArray<FVector> locations, FVector offset, float delay) {
	for (int i = 0; i < locations.Num(); i++)
		SpawnDebugGrid_SetIsSpawn(locations[i] + offset, delay);
}
void UBaseAIBehaviour::SpawnDebugGrid_SetIsObstacle(TArray<FVector> locations, FVector offset, float delay) {
	for (int i = 0; i < locations.Num(); i++)
		SpawnDebugGrid_SetIsObstacle(locations[i] + offset, delay);
}

TScriptInterface<ISvChar> UBaseAIBehaviour::GetClosestCharacter() {

	TScriptInterface<ISvChar> svChar;
	float closestDistance = FLT_MAX;

	auto thisLoc = ThisEnemy->GetActorLocation();

	auto all = GetAllCharacters();
	for (int i = 0; i < all.Num(); i++) {
		if (all[i]) {
			auto actor = all[i]->GetAsActor();
			auto details = actor->GetComponentByClass <UCharacterDetailsComponent>();

			if (details->GetIsDead())
				continue;

			auto thisLocation = actor->GetActorLocation();
			auto distance = FVector::Dist(thisLocation, thisLoc);

			if (distance < closestDistance) {
				svChar = all[i];
				closestDistance = distance;
			}
		}
	}

	return svChar;
}
#pragma optimize("", off)
bool UBaseAIBehaviour::GetIsInFog() {
	if (!GetThisEnemy())
	{
		UDebugMessages::LogError(this, "enemy is null");
		return true;
	}

	auto enemyLocation = GetThisEnemy()->GetActorLocation();

	TArray<FHitResult>hits;
	//GetThisEnemy()->GetWorld()->LineTraceSingleByChannel(hit, enemyLocation, enemyLocation + FVector(0, 0, 200), USvUtilities::GetFogCollisionObjectChannel());
	//DrawDebugLine(GetThisEnemy()->GetWorld(), enemyLocation, enemyLocation + FVector(0, 0, 200), FColor::Red, false, 50, 0, 0);
	FCollisionObjectQueryParams params;
	params.AddObjectTypesToQuery(USvUtilities::GetFogCollisionObjectChannel());

	GetThisEnemy()->GetWorld()->LineTraceMultiByObjectType(hits, enemyLocation + FVector(0, 0, 1000), enemyLocation, params);
	//GetThisEnemy()->GetWorld()->LineTraceSingleByChannel(hit, enemyLocation + FVector(0, 0, 1000), enemyLocation, USvUtilities::GetFogCollisionObjectChannel());
	DrawDebugLine(GetThisEnemy()->GetWorld(), enemyLocation + FVector(0, 0, 1000), enemyLocation, FColor::Red, false, 50, 0, 0);

	for (int i = 0; i < hits.Num(); i++) {
		if (hits[i].GetComponent()) {
			UDebugMessages::LogWarning(this, "multi object trace hit " + hits[i].GetComponent()->GetName());
			auto component = Cast<UFogSectionComponent>(hits[i].GetComponent());

			if (component) {
				return component->GetIsInFog();
			}
		}
	}

	/*if (!hit.GetComponent())
	{
		UDebugMessages::LogError(this, "failed to hit a fog component");
		return true;
	}*/


	return true;
}
#pragma optimize("", on)

bool UBaseAIBehaviour::CanMeleeAnyone() {

	auto allCharacters = GetAllCharacters();

	for (int i = 0; i < allCharacters.Num(); i++) {
		if (allCharacters[i]) {
			if (USvUtilities::AreGridLocationsAdjacent(
				UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetActorLocation()),
				UGridUtilities::GetNormalisedGridLocation(allCharacters[i]->GetAsActor()->GetActorLocation())))
			{
				SetBehaviourTarget(allCharacters[i]);
				return true;
			}
		}
	}

	return false;
}

void UBaseAIBehaviour::BeginDestroy() {
	Super::BeginDestroy();
	ClearInternalFlags(EInternalObjectFlags::Async);
}