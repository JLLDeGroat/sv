// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Enums/EEnvironmentEnums.h"
#include "EnvironmentSpawnerActor.generated.h"

UCLASS()
class SV_API AEnvironmentSpawnerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnvironmentSpawnerActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere) EConstructionType ConstructionType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bShouldSpawnIntel;
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* RootMesh;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
