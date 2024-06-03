// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeIndicatorActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SV_API AGrenadeIndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenadeIndicatorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) USphereComponent* SphereComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* MeshComponent;

	//UPrimitiveComponent, OnComponentEndOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex

	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void OverlapEnded(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetVisibility(bool val);

	void SetRange(float range);
	float GetRange();

private:
	UPROPERTY() float Range;



};
