// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "RetargetLocationComponent.generated.h"

/**
 * 
 */
UCLASS()
class SV_API URetargetLocationComponent : public USphereComponent
{
	GENERATED_BODY()

public:

	URetargetLocationComponent(const FObjectInitializer& ObjectInitializer);
	
};
