// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UActionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionsComponent();

	void SendActionsToUI();

	void SetCanExtract(bool val);
	bool GetCanExtract();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:

	UPROPERTY() bool bCanExtract;
		
};
