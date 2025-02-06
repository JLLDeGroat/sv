// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "../../Enums/EConfigurationEnums.h"
#include "ClimbLadderComponent.generated.h"

/**
 *
 */
UCLASS()
class SV_API UClimbLadderComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:

	UClimbLadderComponent(const FObjectInitializer& ObjectInitializer);

	void StartClimbingLadder(EMovementType movementType, FVector climbLocation, EClimbType climbType);
	void SetMoveUp(bool val);
	void SetPostClimb(bool val);
	void FinishUp();

	void SetPreFallMove(bool val);
	void SetFallMove(bool val);
protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY() bool MoveUp;
	UPROPERTY() bool PreClimbing;
	UPROPERTY() FVector PreClimbLocation;

	UPROPERTY() bool PostClimbing;
	UPROPERTY() FVector PostClimbLocation;

	UPROPERTY() EClimbType CurrentClimbType;

	UPROPERTY() bool PreFallMove;
	UPROPERTY() bool FallMove;

private:

	void ClimbUpTick(float DeltaTime);
	void ClimbDownTick(float DeltaTime);
};
