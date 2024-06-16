// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseActionComponent.h"
#include "LeftClickAction.generated.h"

class USelectionManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API ULeftClickAction : public UBaseActionComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULeftClickAction(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	void DoAction();
	void DoWorldAction();

		
};
