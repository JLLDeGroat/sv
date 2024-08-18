// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../EnvironmentActor.h"
#include "CornerTable.generated.h"

/**
 * 
 */
UCLASS()
class SV_API ACornerTable : public AEnvironmentActor
{
	GENERATED_BODY()
	
public:

	ACornerTable(const FObjectInitializer& ObjectInitializer);
protected:

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* TableMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* TableLegMesh1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* TableLegMesh2;
};
