// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Movable.generated.h"


class UGridMovementComponent;
/**
 *
 */
UINTERFACE(BlueprintType)
class SV_API UMovable : public UInterface
{
	GENERATED_BODY()

};

class SV_API IMovable {

	GENERATED_BODY()

public:

	virtual UGridMovementComponent* GetGridMovementComponent() = 0;

};
