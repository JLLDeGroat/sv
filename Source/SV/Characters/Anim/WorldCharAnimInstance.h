// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WorldCharAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class SV_API UWorldCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UWorldCharAnimInstance(const FObjectInitializer& ObjectInitializer);
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetIsMoving(bool val);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool bIsMoving;
};
