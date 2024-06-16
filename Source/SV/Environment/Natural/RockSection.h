// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "RockSection.generated.h"

/**
 * 
 */
UCLASS()
class SV_API ARockSection : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	ARockSection(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;


protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent* Root;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* Rock1Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* Rock2Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* Rock3Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* Rock4Mesh;

private:

	void DictatePosition();
	void DictateRotation();
};
