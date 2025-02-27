// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "../../../Enums/EEnvironmentEnums.h"
#include "IndicatorActivatorComponent.generated.h"

class UStaticMeshComponent;

UCLASS()
class SV_API UIndicatorActivatorComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UIndicatorActivatorComponent(const FObjectInitializer& ObjectInitializer);

	void SetIndicatorType(EIndicatorType indicatorType);
	void SetMaterialColour(FLinearColor color);

	void ActivateIndicator(AActor* activateOn);
	void DeactivateIndicator(AActor* activateOn);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void OverlapEnded(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* BottomRingMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* TopRingMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UMaterialInstanceDynamic* IndicatorMaterial;

	UPROPERTY() bool bIsActivated;
	UPROPERTY() float Speed;
	UPROPERTY() float DefaultSpeed;

	UFUNCTION() void StartTickAgain();
	FTimerHandle StartTickHandle;

	UPROPERTY() EIndicatorType IndicatorType;
};
