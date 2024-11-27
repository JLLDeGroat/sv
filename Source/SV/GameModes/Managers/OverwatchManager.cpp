// Fill out your copyright notice in the Description page of Project Settings.


#include "OverwatchManager.h"
#include "../../Characters/Components/AttackComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Player/Components/PawnCameraComponent.h"

#pragma optimize("", off)
UOverwatchManager::UOverwatchManager(const FObjectInitializer& ObjectInitializer)
	: UObject(ObjectInitializer) {

	bCurrentlyOverwatchAnimating = false;
}

void UOverwatchManager::AddToOverwatchPotentials(AActor* overwatcher, AActor* target, UPrimitiveComponent* componentSaw) {

	auto attackComponent = overwatcher->GetComponentByClass<UAttackComponent>();

	if (!attackComponent)
		return UDebugMessages::LogError(this, "failed to get attack component");

	/*auto result = attackComponent->TryActivateOverwatch(target, componentSaw);*/

	if (OverwatchItems.Num() == 0) {
		//first attempt at overwatch actor, needs to setup the return location;
		auto controller = GetWorld()->GetFirstPlayerController();
		auto pawnCameraComponent = controller->GetPawn()->GetComponentByClass<UPawnCameraComponent>();
		pawnCameraComponent->SetOverwatchReturnLocation(pawnCameraComponent->GetOwner()->GetActorLocation());

		UDebugMessages::LogDisplay(this, "set component return location");
	}

	auto overwatchItem = FOverwatchItem(overwatcher, target, componentSaw);
	OverwatchItems.Emplace(overwatchItem);

	AttemptToStartAnOverwatch();
}

void UOverwatchManager::DelayTimerHandleCallback() {
	if (bCurrentlyOverwatchAnimating) {
		UDebugMessages::LogWarning(this, "delay timer handle callback fired, but its not currently overwatch animating... not doing anything until its finished");
		return;
	}
	AttemptToStartAnOverwatch();
}

void UOverwatchManager::OnOverwatchItemComplete() {
	UDebugMessages::LogDisplay(this, "OnOverwatchItemComplete");
	auto current = GetCurrentOverwatchItem();
	current->SetIsInComplete(true);
	current->SetIsInProgress(false);

	/*for (int i = 0; i < OverwatchItems.Num(); i++)
		if (OverwatchItems[i].GetIsInProgress())
		{
			UDebugMessages::LogDisplay(this, "removing from overwatch items " + OverwatchItems[i].GetOverwatcher()->GetName());
			OverwatchItems[i].SetIsInComplete(true);
			OverwatchItems[i].SetIsInProgress(false);
			break;
		}*/

	bCurrentlyOverwatchAnimating = false;

	auto stillHasOverwatchesOnTarget = false;
	for (int i = 0; i < OverwatchItems.Num(); i++) {
		if (!OverwatchItems[i].GetIsInComplete() && OverwatchItems[i].GetTarget() == current->GetTarget()) {
			stillHasOverwatchesOnTarget = true;
		}
	}

	if (!stillHasOverwatchesOnTarget) {
		UDebugMessages::LogDisplay(this, "no other targeting this target, will allow it to move at normal speed");
		auto attackComponent = current->GetOverwatcher()->GetComponentByClass<UAttackComponent>();
		if (attackComponent)
			attackComponent->ReturnCharacterAnimationSpeedsToNormal();
	}

	AttemptToStartAnOverwatch();
}

void UOverwatchManager::AttemptToStartAnOverwatch() {
	if (bCurrentlyOverwatchAnimating) {
		UDebugMessages::LogWarning(this, "delay timer handle callback fired, but its not currently overwatch animating... not doing anything until its finished");
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UOverwatchManager::DelayTimerHandleCallback, .25f, false);
		return;
	}

	if (OverwatchItems.Num() > 0) {
		for (int i = 0; i < OverwatchItems.Num(); i++) {
			if (!OverwatchItems[i].GetIsInComplete() && !OverwatchItems[i].GetIsInProgress()) {
				auto attackComponent = OverwatchItems[i].GetOverwatcher()->GetComponentByClass<UAttackComponent>();
				auto result = attackComponent->TryActivateOverwatch(OverwatchItems[i].GetTarget(), OverwatchItems[i].GetComponentSaw());

				if (result == EOverwatchResult::OR_Started) {
					UDebugMessages::LogDisplay(this, "overwatch has started");
					OverwatchItems[i].SetIsInProgress(true);
					bCurrentlyOverwatchAnimating = true;
				}
				else if (result == EOverwatchResult::OR_Delay) {
					GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UOverwatchManager::DelayTimerHandleCallback, .25f, false);
				}
			}
		}
	}
}

bool UOverwatchManager::GetIsCurrentlyOverwatchAnimating() {
	return bCurrentlyOverwatchAnimating;
}

FOverwatchItem* UOverwatchManager::GetCurrentOverwatchItem() {
	for (int i = 0; i < OverwatchItems.Num(); i++) {
		if (OverwatchItems[i].GetIsInProgress()) return &OverwatchItems[i];
	}
	return nullptr;
}
#pragma optimize("", on)