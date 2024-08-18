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
	
	virtual void BeginDestroy() override;
protected:

	UFUNCTION(CallInEditor) void GenericLevel();
	UFUNCTION(CallInEditor) void TwoBuildingLevel();

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* RootMesh;


private:

	void TearDownCurrentGen();

	UPROPERTY() UBaseRunnable* BaseRunnable;

};
