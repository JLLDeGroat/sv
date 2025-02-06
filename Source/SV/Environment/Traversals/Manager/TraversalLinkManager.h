// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "TraversalLinkManager.generated.h"

/**
 *
 */
UCLASS()
class SV_API ATraversalLinkManager : public ANavLinkProxy
{
	GENERATED_BODY()

public:

	ATraversalLinkManager(const FObjectInitializer& ObjectInitializer);

protected:

	UFUNCTION() void OnAddNewLink(FVector start, FVector end);

	virtual void BeginDestroy() override;

};
