// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToggleMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UToggleMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UToggleMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Setup(UStaticMeshComponent* staticMesh, FVector location1, FVector location2);
	void ActivateToggle();

	UFUNCTION(CallInEditor) void EditorToggle();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY() FVector Location1;
	UPROPERTY() FVector Location2;

	UPROPERTY() FVector MovingToLocation;

	UPROPERTY() UStaticMeshComponent* MovingMesh;
};
