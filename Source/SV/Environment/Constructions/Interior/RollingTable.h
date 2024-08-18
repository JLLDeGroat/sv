// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../EnvironmentActor.h"
#include "RollingTable.generated.h"

/**
 *
 */
UCLASS()
class SV_API ARollingTable : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	ARollingTable(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* RollingTable;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* Wheel1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* Wheel2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* Wheel3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* Wheel4;
};
