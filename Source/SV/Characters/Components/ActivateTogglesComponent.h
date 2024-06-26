// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActivateTogglesComponent.generated.h"

class UToggleMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UActivateTogglesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActivateTogglesComponent();
	

	void AddToggleComponent(UToggleMovementComponent* toggleComponent);
	void RemoveToggleComponent(UToggleMovementComponent* toggleComponent);

	TArray<UToggleMovementComponent*> GetToggleComponents();

protected:

	UPROPERTY() TArray<UToggleMovementComponent*> ToggleComponents;
		
};
