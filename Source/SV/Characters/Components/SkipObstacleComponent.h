// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "SkipObstacleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API USkipObstacleComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkipObstacleComponent(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ActivateSkip();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
