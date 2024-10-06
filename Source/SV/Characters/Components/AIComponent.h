// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "../../Enums/EAIEnums.h"
#include "AIComponent.generated.h"

class USphereComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UAIComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAIComponent(const FObjectInitializer& ObjectInitializer);

	void BroadcastAiCompletedTask();

	void SetActivationRadius(float radius);

	void SetIsAiActive(bool val);
	bool GetIsActiveAi();

	void SetAttackRoute(EAIBehaviourAttackRoutes aRoute);
	void SetMovementRoute(EAIBehaviourMoveRoutes mRoute);

	EAIBehaviourAttackRoutes GetAttackRoute();
	EAIBehaviourMoveRoutes GetMovementRoute();



protected:
	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

protected:

	UPROPERTY() bool bIsActiveAI;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USphereComponent* AiActivationSphere;

	UFUNCTION() void OnDelayFinished();
	FTimerHandle DelayFinish;

	UPROPERTY(BlueprintreadWrite, EditAnywhere) float Radius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) EAIBehaviourAttackRoutes AIAttackRoute;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) EAIBehaviourMoveRoutes AIMoveRoute;

};
