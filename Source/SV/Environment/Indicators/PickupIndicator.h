// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseIndicatorActor.h"
#include "PickupIndicator.generated.h"

class UIndicatorActivatorComponent;
class UIndicatorLinkComponent;

UCLASS()
class SV_API APickupIndicator : public ABaseIndicatorActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UIndicatorLinkComponent* LinkComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) AActor* ActorIndicatingTo;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
