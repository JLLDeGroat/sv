// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogManager.generated.h"

class UBoxComponent;
class UFogSectionComponent;
class USceneComponent;

USTRUCT()
struct FFogComponent
{
	GENERATED_BODY()
public:
	FFogComponent()
	{
	}
	FFogComponent(UFogSectionComponent *comp)
	{
		MeshComponent = comp;
	}

	UFogSectionComponent *GetMeshComponent() { return MeshComponent; }

protected:
	UPROPERTY()
	UFogSectionComponent *MeshComponent;
};

UCLASS()
class SV_API AFogManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFogManager();

	void AddComponentAtLocation(FVector loc);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent *SceneRoot;

	UPROPERTY()
	TArray<FFogComponent> FogComponents;
	UPROPERTY()
	TArray<FVector> LocationsToAdd;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UFogSectionComponent *> FogSections;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bActivateOnStartup;

private:
	void AddComponent();
	UPROPERTY()
	UMaterial *MaterialAsset;
	UPROPERTY()
	UStaticMesh *MeshAsset;
	TMap<FVector, int32> FogLocations;

	UPROPERTY()
	FString FogParameterName = "FogAmount";
	UPROPERTY()
	float NoFog = 0.01f;
	UPROPERTY()
	float Fog = 1.0f;
	UPROPERTY()
	float FullFog = 5.0f;
};
