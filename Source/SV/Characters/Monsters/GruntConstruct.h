// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseConstruct.h"
#include "../../Interfaces/Movable.h"
#include "GruntConstruct.generated.h"


/**
 * 
 */
UCLASS()
class SV_API AGruntConstruct : public ABaseConstruct, public IMovable
{
	GENERATED_BODY()
public:

	AGruntConstruct(const FObjectInitializer& ObjectInitializer);

protected:

	virtual UGridMovementComponent* GetGridMovementComponent() override;

};
