// Fill out your copyright notice in the Description page of Project Settings.


#include "AiRangeAttack.h"
#include "../../../Characters/Components/TargetingComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Interfaces/SvChar.h"
#include "../../../Characters/Components/AttackComponent.h"
#include "../../../Characters/Components/HitBoxComponent.h"
#include "../../../Characters/Components/HitCapsuleComponent.h"

UAiRangeAttack::UAiRangeAttack(const FObjectInitializer& ObjectInitializer)
	:UBaseAIBehaviour(ObjectInitializer) {

}
#pragma optimize("", off)
void UAiRangeAttack::DoBehaviour() {

	auto target = GetThisEnemy()->GetComponentByClass<UTargetingComponent>();
	if (!target) {
		UDebugMessages::LogError(this, "failed to get targeting component");
	}

	target->DetermineTargetData();

	auto targetData = target->GetCurrentTargetData();
	if (targetData.Num() > 0) {
		UDebugMessages::LogDisplay(this, "found taget data");
		auto thisTarget = &targetData[0];
		auto attackComponent = GetThisEnemy()->GetComponentByClass<UAttackComponent>();
		if (!attackComponent)
			UDebugMessages::LogError(this, "could not find attack component");
		else {
			auto targetLocation = GetWhereToShootCharacter(thisTarget->GetCharacter(), thisTarget->GetShootLocation());
			attackComponent->TryAttackLocation(thisTarget->GetShootLocation(), targetLocation, 25);
		}
	}
	else UDebugMessages::LogError(this, "failed to get a target");

	/*for (int i = 0; i < targetData.Num(); i++) {
		UDebugMessages::LogDisplay(this, targetData[i].GetCharacter()->GetAsActor()->GetName());
	}*/

	//SetCompletedBehaviour();
}

FVector UAiRangeAttack::GetWhereToShootCharacter(TScriptInterface<ISvChar> character, FVector startLocation) {

	TArray<FVector> locationsToTry;
	FVector bodyLocation1 = FVector::ZeroVector;
	FVector bodyLocation2 = FVector::ZeroVector;

	auto allComps = character->GetAsActor()->GetComponents();
	for (UActorComponent* comp : allComps) {
		if (comp->IsA<UHitBoxComponent>())
		{
			auto  hitBoxComp = (UHitBoxComponent*)comp;

			FHitResult hitResult;
			GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, hitBoxComp->GetComponentLocation(), ECC_Visibility);

			if (hitResult.bBlockingHit) {
				if (hitResult.GetActor() == hitBoxComp->GetOwner() || hitResult.GetActor() == GetThisEnemy()) {
					locationsToTry.Emplace(hitBoxComp->GetComponentLocation());

					if (bodyLocation1 == FVector::ZeroVector) bodyLocation1 = hitBoxComp->GetComponentLocation();
					else if (bodyLocation2 == FVector::ZeroVector) bodyLocation2 = hitBoxComp->GetComponentLocation();
				}
			}
			else
				locationsToTry.Emplace(hitBoxComp->GetComponentLocation());
		}
		else if (comp->IsA<UHitCapsuleComponent>())
		{
			auto hitCapComp = (UHitCapsuleComponent*)comp;

			FHitResult hitResult;
			GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, hitCapComp->GetComponentLocation(), ECC_Visibility);

			if (hitResult.bBlockingHit) {
				if (hitResult.GetActor() == hitCapComp->GetOwner() || hitResult.GetActor() == GetThisEnemy()) {
					locationsToTry.Emplace(hitCapComp->GetComponentLocation());
				}
			}
			else
				locationsToTry.Emplace(hitCapComp->GetComponentLocation());

			locationsToTry.Emplace(hitCapComp->GetComponentLocation());
		}
	}

	if (bodyLocation1 != FVector::ZeroVector) return bodyLocation1;
	else return locationsToTry[GetRandomStream().RandRange(0, locationsToTry.Num() - 1)];
}
#pragma optimize("", on)