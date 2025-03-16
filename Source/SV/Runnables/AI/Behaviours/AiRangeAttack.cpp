// Fill out your copyright notice in the Description page of Project Settings.

#include "AiRangeAttack.h"
#include "../../../Characters/Components/TargetingComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Interfaces/SvChar.h"
#include "../../../Characters/Components/AttackComponent.h"
#include "../../../Characters/Components/HitBoxComponent.h"
#include "../../../Characters/Components/HitCapsuleComponent.h"
#include "../../../Characters/Components/FogHandlerComponent.h"
#include "../../../Player/Components/PawnCameraComponent.h"
#include "../../../Environment/Destructibles/Components/DestructibleHitComponent.h"
#include "../../../Environment/Fog/FogManager.h"
#include "../../../Interfaces/HitComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "Algo/Transform.h"
#include "Algo/Copy.h"

UAiRangeAttack::UAiRangeAttack(const FObjectInitializer &ObjectInitializer)
	: UBaseAIBehaviour(ObjectInitializer)
{
}
#pragma optimize("", off)
void UAiRangeAttack::DoBehaviour()
{
	auto target = GetThisEnemy()->GetComponentByClass<UTargetingComponent>();
	if (!target)
	{
		UDebugMessages::LogError(this, "failed to get targeting component");
	}

	target->DetermineTargetData();

	auto targetData = target->GetCurrentTargetData();
	if (targetData.Num() > 0)
	{
		UDebugMessages::LogDisplay(this, "found taget data");
		auto thisTarget = &targetData[0];
		auto attackComponent = GetThisEnemy()->GetComponentByClass<UAttackComponent>();
		if (!attackComponent)
			UDebugMessages::LogError(this, "could not find attack component");
		else
		{
			auto targetLocation = GetWhereToShootCharacter(thisTarget->GetCharacter(), thisTarget->GetShootLocation());

			if (!targetLocation.GetIsValid())
			{
				UDebugMessages::LogError(this, "failed to get valid target locations");
				SetCompletedBehaviour();
				return;
			}

			attackComponent->TryAttackLocation(
				thisTarget->GetShootLocation(),
				targetLocation.GetLocation(),
				targetLocation.GetPassesThroughDestructionMesh() ? 35 : 25);

			auto pawnCamera = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UPawnCameraComponent>();
			pawnCamera->UpdateCameraState(ECameraState::CS_ReTarget, (GetThisEnemy()->GetActorLocation() + targetLocation.GetLocation()) / 2);

			auto fogHandler = GetThisEnemy()->GetComponentByClass<UFogHandlerComponent>();
			if (!fogHandler)
				UDebugMessages::LogError(this, "no fog handler on this enemy");
			else
			{
				FFunctionGraphTask::CreateAndDispatchWhenReady(
					[fogHandler]
					{ fogHandler->EnableQuickForAiRangeAttack(); },
					TStatId(), nullptr, ENamedThreads::GameThread);
			}
		}
	}
	else
	{
		UDebugMessages::LogError(this, "failed to get a target");
		UDebugMessages::LogError(this, "TODO: need to check on overwatch");
		SetCompletedBehaviour();
	}
}

FWhereToShoot UAiRangeAttack::GetWhereToShootCharacter(TScriptInterface<ISvChar> character, FVector startLocation)
{
	TArray<FWhereToShoot> locationsToShoot;
	TArray<FVector> locationsToTry;
	FVector bodyLocation1 = FVector::ZeroVector;
	FVector bodyLocation2 = FVector::ZeroVector;

	auto allComps = character->GetAsActor()->GetComponents();
	for (UActorComponent *comp : allComps)
	{
		if (comp->Implements<UHitComponent>())
		{
			auto newItem = DeterminePossibleShootLocation(startLocation, comp);
			if (newItem.GetIsValid())
				locationsToShoot.Emplace(newItem);
		}
	}

	TArray<FWhereToShoot> preferredLocations = locationsToShoot.FilterByPredicate(
		[](const FWhereToShoot &thisThing)
		{ return thisThing.GetIsPreferredLocation(); });

	if (preferredLocations.Num() > 0)
		return preferredLocations[GetRandomStream().RandRange(0, preferredLocations.Num() - 1)];
	else if (locationsToShoot.Num() > 0)
		return locationsToShoot[GetRandomStream().RandRange(0, locationsToShoot.Num() - 1)];

	return FWhereToShoot();
}

FWhereToShoot UAiRangeAttack::DeterminePossibleShootLocation(FVector startLocation, TScriptInterface<IHitComponent> destinationHitComponent)
{
	TArray<FHitResult> hitResults;
	FWhereToShoot whereToShoot;
	whereToShoot.SetLocation(destinationHitComponent->GetHitLocation());
	whereToShoot.SetIsPreferredLocation(destinationHitComponent->GetIsPreferredHitLocation());
	whereToShoot.SetIsValid();

	GetWorld()->LineTraceMultiByChannel(hitResults, startLocation, destinationHitComponent->GetHitLocation(), USvUtilities::GetBulletCollisionChannel());

	//DrawDebugLine(GetWorld(), startLocation, destinationHitComponent->GetHitLocation(), FColor::Green, false, 60, 0, 5);

	UDebugMessages::LogDisplay(this, "started possible shoot locations.");
	for (FHitResult hit : hitResults)
	{
		if (hit.bBlockingHit)
		{
			if (hit.GetActor() && !hit.GetActor()->GetComponentByClass<UDestructibleHitComponent>())
			{
				UDebugMessages::LogWarning(this, "found invalid " + hit.GetActor()->GetName());
				whereToShoot.SetIsInvalid();
			}
			else if (hit.GetActor() && hit.GetActor()->GetComponentByClass<UDestructibleHitComponent>())
			{
				UDebugMessages::LogWarning(this, "found destructible " + hit.GetActor()->GetName());
				whereToShoot.SetPassesThroughDestructionMesh(true);
			}
			else
			{
				UDebugMessages::LogWarning(this, "blocking hit against " + hit.GetActor()->GetName());
				if (AFogManager *forManager = Cast<AFogManager>(hit.GetActor()))
				{
					UDebugMessages::LogWarning(this, "disabled by fog");
				}
			}
		}
		else
		{
			UDebugMessages::LogWarning(this, "non blocking hit on " + hit.GetActor()->GetName());
		}
	}
	UDebugMessages::LogDisplay(this, "ended possible shoot locations.");

	return whereToShoot;
}

#pragma optimize("", on)