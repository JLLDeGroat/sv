// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldDelegates.generated.h"
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldMovementComplete, FVector2D, MovedToLocationo);

UCLASS()
class SV_API UWorldDelegates : public UObject
{
	GENERATED_BODY()
public:
	static UWorldDelegates* GetInstance() {
		static UWorldDelegates* worldDelegates = 0; if (!worldDelegates)
		{
			worldDelegates = NewObject<UWorldDelegates>();
			worldDelegates->AddToRoot();
		}
		return worldDelegates;
	}

	FOnWorldMovementComplete _OnWorldMovementComplete;
};
