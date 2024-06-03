// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "AnimSpeedComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UAnimSpeedComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAnimSpeedComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SlowAnimation(float speed = .05f);
	void ReturnToNormalAnimSpeed();

private:

};
