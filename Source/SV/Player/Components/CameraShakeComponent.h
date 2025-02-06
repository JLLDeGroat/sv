// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraShakeComponent.generated.h"

class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UCameraShakeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCameraShakeComponent();

	void StartGunFireShake();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) TSubclassOf<UCameraShakeBase> GunFireShakeComponent;

};
