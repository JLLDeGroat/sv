// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseIndicatorActor.h"
#include "ExtractionIndicator.generated.h"

UCLASS()
class SV_API AExtractionIndicator : public ABaseIndicatorActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExtractionIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:


protected:


};
