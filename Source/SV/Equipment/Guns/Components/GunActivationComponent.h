// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunActivationComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UGunActivationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGunActivationComponent();

protected:

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupForEngineSpinUpActivation(TArray<UStaticMeshComponent*> meshes);


	void ActivateGun();
	void DeactivateGun();

private:

	UPROPERTY() bool bUseEngineSpinup;
	UPROPERTY() TMap<UStaticMeshComponent*, float> ActivationMeshAndSpeed;

	void UseEngineSpinupTick();

};
