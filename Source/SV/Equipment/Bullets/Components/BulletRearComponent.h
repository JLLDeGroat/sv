// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BulletRearComponent.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UBulletRearComponent : public USphereComponent
{
	GENERATED_BODY()

public:

	UBulletRearComponent(const FObjectInitializer& ObjectInitializer);
	
};
