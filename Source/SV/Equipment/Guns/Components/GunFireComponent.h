// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunFireComponent.generated.h"

class UStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UGunFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunFireComponent();

	//Accuracy of 1 is default, multiplier is multiplied by distance 
	void FireAtLocation(FVector location, float accuracyRadius = 0);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMeshAndSocketName(UStaticMeshComponent* meshComponent, FString socketName);
private:

	FVector GetGunFireStartLocation();

	UPROPERTY() UStaticMeshComponent* GunFireStartMesh;
	UPROPERTY() FString GunFireStartSocketName;

	UPROPERTY() FRandomStream Random;
		
};
