// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePrefab.generated.h"

UCLASS()
class SV_API ABasePrefab : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABasePrefab(const FObjectInitializer &ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
