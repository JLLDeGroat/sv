// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "RightClickAction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API URightClickAction : public UBaseActionComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URightClickAction(const FObjectInitializer& ObjectInitializer);
	void DoAction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
