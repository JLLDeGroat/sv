// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentDetailsComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UEnvironmentDetailsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnvironmentDetailsComponent();

	void SetThickness(int value);
	int GetThickness();

	void SetAffectsFog(bool affectsFog);
	bool GetAffectsFog();
protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Thickness;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool AffectsFog;

};
