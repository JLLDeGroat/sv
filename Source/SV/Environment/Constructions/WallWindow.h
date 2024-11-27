// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "WallWindow.generated.h"

class UVaultableComponent;
class UEnvironmentDetailsComponent;
/**
 * 
 */
UCLASS()
class SV_API AWallWindow : public AEnvironmentActor
{
	GENERATED_BODY()

public:

	AWallWindow(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* FenceMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UVaultableComponent* VaultableComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UEnvironmentDetailsComponent* DetailsComponent;
};
