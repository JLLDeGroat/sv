// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "VaultObstacleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UVaultObstacleComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVaultObstacleComponent(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ActivateVault();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
