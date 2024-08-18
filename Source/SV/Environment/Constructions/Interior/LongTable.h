// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../EnvironmentActor.h"
#include "LongTable.generated.h"

/**
 * 
 */
UCLASS()
class SV_API ALongTable : public AEnvironmentActor
{
	GENERATED_BODY()
	
public:

public:
	// Sets default values for this actor's properties
	ALongTable(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* Wheel1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* Wheel2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* Wheel3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* Wheel4;
};
