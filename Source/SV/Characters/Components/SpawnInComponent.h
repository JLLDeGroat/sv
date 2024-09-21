// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "SpawnInComponent.generated.h"

/**
 * 
 */
UCLASS()
class SV_API USpawnInComponent : public UAnimAccessComponent
{
	GENERATED_BODY()
	
public:

	USpawnInComponent(const FObjectInitializer& ObjectInitializer);
	void StartSpawnInFromGround();
	void UpdateMovementSpeed(float speed);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

protected:

	UPROPERTY() FVector EndLocation;
	UPROPERTY() float MovementUpSpeed;
};
