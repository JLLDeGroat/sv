// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterTileUIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UCharacterTileUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterTileUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
	UFUNCTION() void BroadCastDelayCallback();
private:
	FTimerHandle BroadCastDelay;
};
