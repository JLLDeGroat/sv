// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtractionIndicator.generated.h"

class UStaticMeshComponent;
class UIndicatorActivatorComponent;

UCLASS()
class SV_API AExtractionIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExtractionIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	/*virtual void Tick(float DeltaTime) override;

	void ActivateIndicator();
	void DeactivateIndicator();*/

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* BaseMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UIndicatorActivatorComponent* ActivatorIndicator;

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* BaseMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* BottomRingMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* TopRingMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBoxComponent* IndicatorActivator;

	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void OverlapEnded(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY() float Speed;
	UPROPERTY() float DefaultSpeed;

	UFUNCTION() void StartTickAgain();
	FTimerHandle StartTickHandle;

	UPROPERTY() bool bIsActivated;*/

};
