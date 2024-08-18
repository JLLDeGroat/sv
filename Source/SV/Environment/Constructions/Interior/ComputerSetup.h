// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../EnvironmentActor.h"
#include "ComputerSetup.generated.h"

/**
 * 
 */
UCLASS()
class SV_API AComputerSetup : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	AComputerSetup(const FObjectInitializer& ObjectInitializer);
	virtual void OnConstruction(const FTransform& Transform) override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* ScreenMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* TowerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* KeyboardMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* MouseMesh;

};
