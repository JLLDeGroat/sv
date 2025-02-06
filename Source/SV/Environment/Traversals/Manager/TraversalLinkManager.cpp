// Fill out your copyright notice in the Description page of Project Settings.


#include "TraversalLinkManager.h"
#include "../../../Delegates/GameplayDelegates.h"
#include "../../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "NavLinkCustomComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

ATraversalLinkManager::ATraversalLinkManager(const FObjectInitializer& ObjectInitializer)
	: ANavLinkProxy(ObjectInitializer) {

	auto gameplayDelegates = UGameplayDelegates::GetInstance();

	if (gameplayDelegates)
		gameplayDelegates->_OnNewLinkAdded.AddDynamic(this, &ATraversalLinkManager::OnAddNewLink);

	PointLinks.Empty();
}
#pragma optimize("", off)
void ATraversalLinkManager::OnAddNewLink(FVector start, FVector end) {
	FNavigationLink link;
	link.Left = start;
	link.Right = end;

	link.Direction = ENavLinkDirection::BothWays;
	PointLinks.Add(link);

	UDebugMessages::LogDisplay(this, "NavLinkUpdated");

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		NavSys->UpdateActorInNavOctree(*this);
		NavSys->Build();

		UDebugMessages::LogDisplay(this, "UpdateActorInNavOctree");
	}
}
#pragma optimize("", on)
void ATraversalLinkManager::BeginDestroy() {
	Super::BeginDestroy();
	PointLinks.Empty();
}