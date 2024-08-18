// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "ResourcePickup.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UPickupDetailsComponent;
/**
 * 
 */
UCLASS()
class SV_API AResourcePickup : public AEnvironmentActor
{
	GENERATED_BODY()
	
public:

	AResourcePickup(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;


protected:

	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* PickupMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBoxComponent* BoxComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UPickupDetailsComponent* PickupDetailsComponent;

};
