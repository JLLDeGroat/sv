// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIBehaviour.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Interfaces/SvChar.h"
#include "../../../Delegates/AIDelegates.h"
#include "../../../World/WorldGridItemActor.h"

UBaseAIBehaviour::UBaseAIBehaviour(const FObjectInitializer& ObjectInitializer)
	: UObject(ObjectInitializer) {
	CompletedBehaviour = false;

	auto aiDelegates = UAIDelegates::GetInstance();
	if (aiDelegates)
		aiDelegates->_AICharacterFinishedBehaviour.AddDynamic(this, &UBaseAIBehaviour::SetCompletedBehaviour);
	else
		UDebugMessages::LogError(this, "failed to bind to delegate");
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