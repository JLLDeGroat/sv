// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseControllerManager.h"
#include "../../Enums/EControllerEnums.h"
#include "ControlManager.generated.h"

class USelectionManager;
class AGrenadeIndicatorActor;
class AGridSelectionActor;
/**
 *
 */
UCLASS()
class SV_API UControlManager : public UBaseControllerManager
{
	GENERATED_BODY()

public:

	UControlManager(const FObjectInitializer& ObjectInitializer);

	void SetGrenadeIndicatorActor(AGrenadeIndicatorActor* indicatorActor);
	FVector GetGrenadeIndicatorActorLocation();

	void SetGridSelectionActor(AGridSelectionActor* gridSelectionActor);
	AGridSelectionActor* GetGridSelectionActor();

	FORCEINLINE bool GetCanMouseDesignateSelectionDecal();
	void SetCanMouseDesignateSelectionDecal(bool val);

	FORCEINLINE bool GetCanMouseDesignateExplosionRadiusActor();
	void SetCanMouseDesignateExplosionRadiusActor(bool val);

	void TickShowGrenadeIndicator(FVector mouseLocation);
	void TickFindMovementPath(FVector localised);

	UFUNCTION() void OnUIItemHovered();
	UFUNCTION() void OnUIItemUnhovered();

	UFUNCTION() void UpdateControlLimit(EControlLimit controlLimit);

	EControlLimit GetControlLimit();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY() bool bCanMouseDesignateSelectionDecal;
	UPROPERTY() bool bCanMouseDesignateExplosionRadiusActor;

	UPROPERTY() AGrenadeIndicatorActor* GrenadeIndicatorActor;
	UPROPERTY() AGridSelectionActor* GridSelectionActor;
	UPROPERTY() USelectionManager* SelectionManager;


	UPROPERTY() bool bDisabledMouseDesignationOnHover;
	UPROPERTY() bool bDisabledMouseExplosionDesignationOnHover;
	UPROPERTY() bool bIsHovered;

	UPROPERTY() EControlLimit ControlLimits;

};
