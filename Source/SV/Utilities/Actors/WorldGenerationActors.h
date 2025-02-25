// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldGenerationActors.generated.h"

class UStaticMeshComponent;
class UBaseRunnable;

UCLASS()
class SV_API AWorldGenerationActors : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldGenerationActors();

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

protected:
	UFUNCTION(CallInEditor, category = "generations")
	void GenericLevel();
	UFUNCTION(CallInEditor, category = "generations")
	void TwoBuildingLevel();

	UFUNCTION(CallInEditor, category = "generations")
	void DestroyAllDebugActors();

	UFUNCTION(CallInEditor, category = "win loss")
	void RunWinLossRunnable();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent *RootMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "generations")
	bool bShouldGenerate;

private:
	void TearDownCurrentGen();

	UPROPERTY()
	UBaseRunnable *BaseRunnable;
};
