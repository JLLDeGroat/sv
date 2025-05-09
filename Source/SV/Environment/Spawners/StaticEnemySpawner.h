// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "StaticEnemySpawner.generated.h"

class UNiagaraComponent;

/**
 *
 */
UCLASS()
class SV_API AStaticEnemySpawner : public AEnvironmentActor
{
	GENERATED_BODY()

public:
	AStaticEnemySpawner(const FObjectInitializer &ObjectInitializer);

	UFUNCTION(CallInEditor)
	void StartSpawnFromGround();
	void StartSpawnFromWall();

	void StartGroundSpawnEffects();
	void StartWallSpawnEffects();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *BaseMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UNiagaraComponent *GroundSmokeComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UNiagaraComponent *WallSmokeComponent;
};
