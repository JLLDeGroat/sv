// Fill out your copyright notice in the Description page of Project Settings.

#include "FogHandlerComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Environment/Fog/Components/FogSectionComponent.h"
#include "../../Environment/Components/EnvironmentDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../../Environment/EnvironmentActor.h"
#include "../../Delegates/CharacterDelegates.h"

#include "../../Characters/Components/HealthAndStatusWidgetComponent.h"
#include "Components/CapsuleComponent.h"

UFogHandlerComponent::UFogHandlerComponent(const FObjectInitializer &ObjectInitializer)
	: USphereComponent(ObjectInitializer)
{

	bIsAi = false;
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionObjectType(USvUtilities::GetFogCollisionObjectChannel());
	SetCollisionResponseToChannel(USvUtilities::GetFogCollisionObjectChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RecreatePhysicsState();
	OnComponentBeginOverlap.AddDynamic(this, &UFogHandlerComponent::Overlapped);
	SetGenerateOverlapEvents(false);
	/*bHiddenInGame = false;
	SetVisibility(true);*/

	auto characterDelegates = UCharacterDelegates::GetInstance();
	if (!characterDelegates)
		UDebugMessages::LogError(this, "failed to get character delegates");
	else
		characterDelegates->_OnFogGenerationComplete.AddDynamic(this, &UFogHandlerComponent::OnFogComplete);
}

void UFogHandlerComponent::Overlapped(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									  int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

	if (GetOwner() == OtherActor) // overlapped with self
		return;

	UFogSectionComponent *fogSection = Cast<UFogSectionComponent>(OtherComp);
	if (fogSection)
	{
		FogSectionComponents.Emplace(fogSection);

		if (ShouldRemoveFog(fogSection))
			fogSection->SetAsNoFog();
	}
	else
	{
		if (UCapsuleComponent *capsule = Cast<UCapsuleComponent>(OtherComp))
		{
			auto healthAnStatusComp = OtherActor->GetComponentByClass<UHealthAndStatusWidgetComponent>();
			if (healthAnStatusComp)
				healthAnStatusComp->ShowComponent();
		}
	}
}
void UFogHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
}

TArray<UFogSectionComponent *> UFogHandlerComponent::GetFogSectionComponents()
{
	return FogSectionComponents;
}

void UFogHandlerComponent::OnFogComplete()
{
	if (!bIsAi)
	{
		SetGenerateOverlapEvents(true);
		UpdateOverlaps();
		// UDebugMessages::LogDisplay(this, "OnFogComplete");
	}
}
void UFogHandlerComponent::EnableQuickForAiRangeAttack()
{
	SetGenerateOverlapEvents(true);
	UpdateOverlaps();
	// UDebugMessages::LogDisplay(this, "Quick fog overlap");
	GetWorld()->GetTimerManager().SetTimer(QuickFogOverlap, this, &UFogHandlerComponent::QuickFogOverlapCallback, false, 1.0f);
}

void UFogHandlerComponent::QuickFogOverlapCallback()
{
	SetGenerateOverlapEvents(false);
}

bool UFogHandlerComponent::ShouldRemoveFog(UFogSectionComponent *fogComponent)
{
	return ShouldRemoveFog(fogComponent, GetOwner()->GetActorLocation());
}

bool UFogHandlerComponent::ShouldRemoveFog(UFogSectionComponent *fogComponent, FVector location)
{
	TArray<FHitResult> hits;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceMultiByChannel(hits, location, fogComponent->GetComponentLocation(), USvUtilities::GetEnvironmentChannel());

	if (hits.IsEmpty())
	{
		return true;
	}
	else
	{
		bool isValid = true;
		for (int i = 0; i < hits.Num(); i++)
		{
			if (hits[i].GetActor()->IsA<AEnvironmentActor>())
			{
				auto detailsComponent = hits[i].GetActor()->GetComponentByClass<UEnvironmentDetailsComponent>();
				if (detailsComponent && detailsComponent->GetAffectsFog())
				{
					isValid = false;
					// UDebugMessages::LogDisplay(this, "failed with hit on " + hits[i].GetActor()->GetName());
				}
			}
		}

		if (isValid)
			return true;
	}

	return false;
}

void UFogHandlerComponent::SetIsAi(bool val)
{
	bIsAi = true;
}
bool UFogHandlerComponent::GetIsAi()
{
	return bIsAi;
}