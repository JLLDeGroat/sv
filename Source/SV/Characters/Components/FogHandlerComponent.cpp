// Fill out your copyright notice in the Description page of Project Settings.


#include "FogHandlerComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Environment/Fog/Components/FogSectionComponent.h"
#include "../../Environment/Components/EnvironmentDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../../Environment/EnvironmentActor.h"
#include "../../Delegates/CharacterDelegates.h"

UFogHandlerComponent::UFogHandlerComponent(const FObjectInitializer& ObjectInitializer)
	: USphereComponent(ObjectInitializer) {


	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(USvUtilities::GetFogCollisionObjectChannel(), ECR_Overlap);
	RecreatePhysicsState();
	OnComponentBeginOverlap.AddDynamic(this, &UFogHandlerComponent::Overlapped);

	/*bHiddenInGame = false;
	SetVisibility(true);*/

	auto characterDelegates = UCharacterDelegates::GetInstance();
	if (!characterDelegates) UDebugMessages::LogError(this, "failed to get character delegates");
	else characterDelegates->_OnFogGenerationComplete.AddDynamic(this, &UFogHandlerComponent::OnFogComplete);
}

void UFogHandlerComponent::Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	UFogSectionComponent* fogSection = Cast<UFogSectionComponent>(OtherComp);

	if (fogSection) {
		FogSectionComponents.Emplace(fogSection);

		if (ShouldRemoveFog(fogSection))
			fogSection->SetAsNoFog();
	}
}
void UFogHandlerComponent::BeginPlay() {
	Super::BeginPlay();
}

TArray<UFogSectionComponent*> UFogHandlerComponent::GetFogSectionComponents() {
	return FogSectionComponents;
}

void UFogHandlerComponent::OnFogComplete() {
	SetGenerateOverlapEvents(true);
	UpdateOverlaps();
	UDebugMessages::LogDisplay(this, "OnFogComplete");
}

bool UFogHandlerComponent::ShouldRemoveFog(UFogSectionComponent* fogComponent) {
	return ShouldRemoveFog(fogComponent, GetOwner()->GetActorLocation());
}

bool UFogHandlerComponent::ShouldRemoveFog(UFogSectionComponent* fogComponent, FVector location) {
	TArray<FHitResult> hits;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceMultiByChannel(hits, location, fogComponent->GetComponentLocation(), USvUtilities::GetEnvironmentChannel());

	if (hits.IsEmpty()) {
		return true;
	}
	else {
		bool isValid = true;
		for (int i = 0; i < hits.Num(); i++) {
			if (hits[i].GetActor()->IsA<AEnvironmentActor>()) {
				auto detailsComponent = hits[i].GetActor()->GetComponentByClass<UEnvironmentDetailsComponent>();
				if (detailsComponent && detailsComponent->GetAffectsFog()) {
					isValid = false;
					UDebugMessages::LogDisplay(this, "failed with hit on " + hits[i].GetActor()->GetName());
				}
			}
		}


		if (isValid)
			return true;
	}

	return false;
}