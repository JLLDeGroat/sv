// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetingIndicatorActor.generated.h"

class USphereComponent;

UCLASS()
class SV_API ATargetingIndicatorActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATargetingIndicatorActor();

	float GetSphereScaledRadius();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USphereComponent* SphereComponent;
};
