// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupIndicatorLink.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UPickupIndicatorLink : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupIndicatorLink();

	void SetIndicatorIndicatingTo(AActor* actor);
	AActor* GetIndicatorIndicatingTo();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) AActor* IndicatorIndicatingTo;

};
