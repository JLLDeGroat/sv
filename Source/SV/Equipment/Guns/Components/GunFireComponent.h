// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunFireComponent.generated.h"

class UStaticMeshComponent;
class ABaseBullet;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UGunFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGunFireComponent();

	//Accuracy of 1 is default, multiplier is multiplied by distance 
	void FireAtLocation(FVector location, float accuracyRadius = 0);
	void SetBulletClass(TSubclassOf<ABaseBullet> clss);
	void SetMeshAndSocketName(UStaticMeshComponent* meshComponent, FString socketName);
private:

	FVector GetGunFireStartLocation();

	UPROPERTY() UStaticMeshComponent* GunFireStartMesh;
	UPROPERTY() FString GunFireStartSocketName;

	UPROPERTY() FRandomStream Random;

	UPROPERTY() TSubclassOf<ABaseBullet> BulletClass;

};
