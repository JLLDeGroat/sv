// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "FogSectionComponent.generated.h"

class UMaterialInstanceDynamic;
/**
 * 
 */
UCLASS()
class SV_API UFogSectionComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	void SetMaterialInstance(UMaterialInstanceDynamic* instance);

	void SetAsNoFog();
	void SetAsFog();
	void SetAsFullFog();

	bool GetIsInFog();

	void InitializeFogSectionProperties(FVector loc);

protected:

	UPROPERTY() UMaterialInstanceDynamic* MaterialInstance;
	
	UPROPERTY() bool bIsInFog;

};
