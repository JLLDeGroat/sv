// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "ThrowExplosionComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UThrowExplosionComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UThrowExplosionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION() void FuseHandleCallback();

	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BeginExplosion();
	void SetFuseTime(float fuseTime);
	void SetExplosionRadius(float radius);

private:

	UPROPERTY() TArray<AActor*> ActorsCaughtInExplosion;

	FTimerHandle FuseHandle;

	UPROPERTY() float FuseTime;
	UPROPERTY() float ExplosionRadius;

};
