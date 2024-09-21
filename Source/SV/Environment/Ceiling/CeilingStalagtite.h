// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "CeilingStalagtite.generated.h"

/**
 *
 */
UCLASS()
class SV_API ACeilingStalagtite : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	ACeilingStalagtite(const FObjectInitializer& ObjectInitializer);


protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent* RootSceneComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* StalagtiteComponent;

	virtual void OnConstruction(const FTransform& Transform) override;

private:

	void InitializeComponent();

};
