// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "FogHandlerComponent.generated.h"

class UFogSectionComponent;
/**
 *
 */
UCLASS()
class SV_API UFogHandlerComponent : public USphereComponent
{
	GENERATED_BODY()

public:

	UFogHandlerComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay();

	TArray<UFogSectionComponent*> GetFogSectionComponents();

	bool ShouldRemoveFog(UFogSectionComponent* fogComponent);
	bool ShouldRemoveFog(UFogSectionComponent* fogComponent, FVector location);

	UFUNCTION() void OnFogComplete();

protected:

	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY() TArray<UFogSectionComponent*>FogSectionComponents;
};
