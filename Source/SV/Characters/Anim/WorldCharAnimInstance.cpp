// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldCharAnimInstance.h"

UWorldCharAnimInstance::UWorldCharAnimInstance(const FObjectInitializer& ObjectInitializer) {
	bIsMoving = false;
}
void UWorldCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UWorldCharAnimInstance::SetIsMoving(bool val) {
	bIsMoving = val;
}