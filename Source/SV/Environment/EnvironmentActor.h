// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class SV_API AEnvironmentActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnvironmentActor(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FORCEINLINE void SetupEnvironmentMeshComponent(UStaticMeshComponent* component);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
