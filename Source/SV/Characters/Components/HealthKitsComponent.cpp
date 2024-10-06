// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthKitsComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Instance/SvGameInstance.h"
#include "../../Instance/Managers/HealthKitDataManager.h"
#include "../../Player/Components/PawnCameraComponent.h"
#include "../Anim/CharAnimInstance.h"
#include "CharacterDetailsComponent.h"
#include "HealthAndStatusWidgetComponent.h"
#include "../../Equipment/Tools/BasicHealthKit.h"

UHealthKitsComponent::UHealthKitsComponent(const FObjectInitializer& ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer) {

}

int UHealthKitsComponent::GetHealthKitAmounts(EHealthKits healthKit) {
	for (int i = 0; i < HealthKits.Num(); i++) {
		if (HealthKits[i].GetHealthKitDataItem()->GetHealthKitType() == healthKit)
			return HealthKits[i].GetAmount();
	}
	return 0;
}

void UHealthKitsComponent::AddHealthKits(EHealthKits healthKit, int amount, FGuid kitId) {
	for (int i = 0; i < HealthKits.Num(); i++) {
		if (HealthKits[i].GetHealthKitDataItem()->GetHealthKitType() == healthKit)
			return HealthKits[i].Add(amount);
	}

	auto instance = USvUtilities::GetGameInstance(GetOwner()->GetWorld());
	auto healthKitData = instance->GetHealthKitDataManager();
	auto dataItem = healthKitData->GetHealthKitData()->GetHealthKit(healthKit);
	HealthKits.Emplace(FHealthKit(*dataItem, 1, kitId));
}

void UHealthKitsComponent::AddKitoPreviouslyUsed(EHealthKits healthKit, int amount, FGuid kitId) {
	for (int i = 0; i < UsedHealthKits.Num(); i++) {
		if (UsedHealthKits[i].GetHealthKitDataItem()->GetHealthKitType() == healthKit)
			return UsedHealthKits[i].Add(amount);
	}

	auto instance = USvUtilities::GetGameInstance(GetOwner()->GetWorld());
	auto healthKitData = instance->GetHealthKitDataManager();
	auto dataItem = healthKitData->GetHealthKitData()->GetHealthKit(healthKit);
	UsedHealthKits.Emplace(FHealthKit(*dataItem, 1, kitId));
}
TArray<FHealthKit> UHealthKitsComponent::GetPreviouslyThrownThrowables() {
	return UsedHealthKits;
}

void UHealthKitsComponent::BeginUseActiveHealthKitOnActor(AActor* actor) {
	ActiveHealthKitRecipient = actor;

	if (ActiveHealthKitRecipient == GetOwner())
		AnimInstance->SetIsHealingSelf(true);
	else
		AnimInstance->SetIsHealingAlly(true);

	if (GetOwner() != ActiveHealthKitRecipient) {
		auto lookatRot = UGridUtilities::FindLookAtRotation(GetOwner()->GetActorLocation(), ActiveHealthKitRecipient->GetActorLocation());
		GetOwner()->SetActorRotation(lookatRot);
	}
}

void UHealthKitsComponent::UseActiveHealthKitOnActor() {
	UDebugMessages::LogDisplay(this, "using healthkit on actor");

	AnimInstance->SetIsHealingSelf(false);
	AnimInstance->SetIsHealingAlly(false);

	auto detailsComponent = ActiveHealthKitRecipient->GetComponentByClass<UCharacterDetailsComponent>();
	if (!detailsComponent)
		return UDebugMessages::LogError(this, "failed to get character details component");

	FHealthKit* healthKit = nullptr;
	for (int i = 0; i < HealthKits.Num(); i++) {
		if (HealthKits[i].GetHealthKitDataItem()->GetHealthKitType() == ActiveHealthKit) {
			healthKit = &HealthKits[i];
			UsedHealthKits.Emplace(FHealthKit(*HealthKits[i].GetHealthKitDataItem(), 1, HealthKits[i].GetHealthKitId()));
			break;
		}
	}

	if (!healthKit)
		return UDebugMessages::LogError(this, "failed to get healthKit");

	detailsComponent->AddHealth(healthKit->GetHealthKitDataItem()->GetHealAmount());

	auto statusComponent = ActiveHealthKitRecipient->GetComponentByClass<UHealthAndStatusWidgetComponent>();
	if (statusComponent) {
		statusComponent->UpdateOnHealthChange();
	}

	if (GetOwner()->GetComponentByClass<UCharacterDetailsComponent>()->GetCharacterControl() == ECharacterControl::CC_Player) {
		auto world = GetOwner()->GetWorld();
		auto controller = world->GetFirstPlayerController();

		auto pawnCamera = controller->GetPawn()->GetComponentByClass<UPawnCameraComponent>();

		if (!pawnCamera)
			return UDebugMessages::LogError(this, "could not find pawn camera component");

		pawnCamera->UpdateCameraState(ECameraState::CS_Control, FVector::ZeroVector, FVector::ZeroVector, true);
	}
}

void UHealthKitsComponent::SetActiveHealthKit(EHealthKits healthKit) {
	ActiveHealthKit = healthKit;
}
EHealthKits UHealthKitsComponent::GetActiveHealthKit() {
	return ActiveHealthKit;
}

void UHealthKitsComponent::SpawnHealthKitOnHand() {
	auto healthKit = GetWorld()->SpawnActor<ABasicHealthKit>();
	auto skeleMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	if (skeleMesh && healthKit) {
		healthKit->AttachToComponent(skeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("LeftHandSocket"));
		healthKit->SetActorRelativeLocation(FVector(14, 6, .3f));
		HealthKitActor = healthKit;
	}
	else
		return UDebugMessages::LogError(this, "failed to get skelemesh or spawn the healthkit");
}
void UHealthKitsComponent::DespawnHealthKitOnHand() {
	if (HealthKitActor) HealthKitActor->Destroy();
}

AActor* UHealthKitsComponent::GetHealthKitActor() {
	return HealthKitActor;
}